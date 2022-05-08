/*#######################################################################################
	Copyright (c) 2018-2022 As Project
	https://github.com/AsPJT/Roguelike_Siv3D_Sample
	wanotaitei@gmail.com

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#pragma once

//描画フラグ管理
struct DrawFlag {
	bool main_map{ true };
	bool sub_map{ true };
	bool tile_middle_leftine{ false };
	bool hp_bar{ true };
};

//イベントフラグ管理
struct EventFlag {
	bool finished_walking{ false }; //1マス移動した時
	bool next_floor{ false }; //次の階へ行く時

	bool die{ false }; //死んだ時
	bool hungry{ false }; //お腹が限界の時

	bool is_next_floor_time{ false };
	bool is_die_time{ false };

	void init() {
		finished_walking = false;
		next_floor = false;
		die = false;
		hungry = false;
	}
};
