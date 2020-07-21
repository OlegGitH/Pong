#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#define after_pressed(b) (input->buttons[b].changed)

// players virable
// player_*_p - it's a size of rectangle
float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;

int player_1_score, player_2_score;


// Game menu, thinking to expand more mode(theme)
enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

// Easy exit the pong game
bool exit_game(bool on_exit) {
	if (on_exit)
		return running = false;
}
Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void 
simulate_player(float* p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	} else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

// bool function to bounce ball from players pad
internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}

internal void
simulate_screen(Input* input, float dt) {

	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x54534e);
	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0xf2dd72);

	// make statement "Are you sure you want to leave the pong game?"
	exit_game(pressed(BUTTON_ESC));

	

	if (current_gamemode == GM_GAMEPLAY) {
		float player_1_ddp = 0.f;

		if (pressed(BUTTON_TAB)) {
			current_gamemode = GM_MENU;
		}

		// take effect before the compilation happens 
		if (!enemy_is_ai) {
		if (is_down(BUTTON_UP)) player_1_ddp += 2000;
		if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
	}
	else {
		//if (ball_p_y > player_1_p) player_1_ddp += 1300;
		//if (ball_p_y < player_1_p) player_1_ddp -= 1300;
		player_1_ddp = (ball_p_y - player_1_p) * 100;
		if (player_1_ddp > 1300) player_1_ddp = 1300;
		if (player_1_ddp < -1300) player_1_ddp = -1300;
	}
		float player_2_ddp = 0.f;
		if (is_down(BUTTON_W)) player_2_ddp += 2000;
		if (is_down(BUTTON_S)) player_2_ddp -= 2000;

		player_1_ddp -= player_1_dp * 10.f;

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

		{
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;

			// Using this "" function to make ball bounce from pad number 1 
			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 84, player_1_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = 82 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;;
			}

			// Using this "" function to make ball bounce from pad number 2
			else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -84, player_2_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = -82 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;;

			}
			// move ball to the center if it had touched x wall
			if (ball_p_x + ball_half_size > arena_half_size_x) {
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_1_score++;
			}
			if (ball_p_x - ball_half_size < -arena_half_size_x) {
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_2_score++;
			}
			// collide ball if it had touch y wall
			if (ball_p_y + ball_half_size > arena_half_size_y) {
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			if (ball_p_y - ball_half_size < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}
		}

		draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
		draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);

		u32	color_theme = 0x575652;
		u32 color_theme_player = 0xa39350;

		// players bar
		draw_rect(.5f, 0, 1, 45, 0xffffff);
		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, color_theme_player);
		draw_rect(82, player_1_p, 1, 12, color_theme_player);
		draw_rect(-82, player_2_p, 1, 12, color_theme_player);

		draw_text("ESC TO LEAVE", -80, arena_half_size_y + 4.5, .2f, 0xffffff);
		draw_text("RETURN TO MENU", -80, arena_half_size_y + 2.5, .2f, 0xffffff);
		if (player_1_score - 50 > player_2_score) {
			draw_text("Player 1 Win", -80, -10, 12, 0xffffff);
		}
	}
	else {

		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
			hot_button = !hot_button;
		}



		if (released(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
			enemy_is_ai = hot_button ? 0 : 1;
		}
		if (hot_button == 0) {
			draw_text("SINGLE PLAYER", -80, -10, 1, 0xff0000);
			draw_text("MULTIPLAYER", 15, -10, 1, 0xaaaaaa);
		}
		else {
			draw_text("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
			draw_text("MULTIPLAYER", 15, -10, 1, 0xff0000);
		}
		
		draw_text("GAME PONG", -50, 30, 2, 0XFFFFFF);


	}

	


}