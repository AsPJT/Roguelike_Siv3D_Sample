/*#######################################################################################
	Copyright (c) 2018-2022 As Project
	https://github.com/AsPJT/Roguelike_Siv3D_Sample
	wanotaitei@gmail.com

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#pragma once


enum : dtl::type::size {
	agent_type_empty,
	agent_type_player,
	agent_type_enemy
};

enum : dtl::type::size {
	walking_type_empty,
	walking_type_direction0,
	walking_type_direction4,
	walking_type_direction8
};

enum : dtl::type::size {
	intelligence_type_empty,
	intelligence_type_player,
	intelligence_type_enemy
};

enum : dtl::type::size {
	behavior_lower_middle,
	behavior_lower_left,
	behavior_middle_left,
	behavior_lower_right,
	behavior_middle_right,
	behavior_upper_left,
	behavior_upper_middle,
	behavior_upper_right,
	behavior_middle,
	behavior_empty
};

enum : dtl::type::size {
	direction_lower_middle,
	direction_lower_left,
	direction_middle_left,
	direction_lower_right,
	direction_middle_right,
	direction_upper_left,
	direction_upper_middle,
	direction_upper_right,
	direction_middle
};

template<typename Int_, typename Float_>
struct Agent {
	
	//基本的な情報
	
	dtl::type::size agent_type{ agent_type_empty }; //エージェントの種類
	dtl::type::size walking_type{ walking_type_empty }; //歩行の種類
	dtl::type::size intelligence_type{ intelligence_type_empty }; //知能の種類

	//個体のステータス

	dtl::type::size level{ 1 }; //レベル
	dtl::type::size hp{ 30 }; //体力
	dtl::type::size max_hp{ 50 }; //最大体力
	dtl::type::size hunger{ 50 }; //腹減りゲージ
	dtl::type::size max_hunger{ 100 }; //最大腹減りゲージ

	dtl::type::size hunger_steps{}; //腹減り歩数のカウント
	dtl::type::size max_hunger_steps{ 3 }; //腹減り歩数

	dtl::Vec2<Int_> position{}; //位置
	Float_ move{}; //細かな位置
	dtl::type::size behavior_status{ behavior_empty }; //動作の状態
	dtl::type::size direction{ direction_lower_middle }; //向き
};
