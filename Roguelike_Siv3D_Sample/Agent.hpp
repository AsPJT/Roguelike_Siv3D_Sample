/*#######################################################################################
	Copyright (c) 2018-2022 As Project
	https://github.com/AsPJT/Roguelike_Siv3D_Sample
	wanotaitei@gmail.com

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#pragma once


enum class AgentType : dtl::type::size {
	empty,
	player,
	enemy
};

enum class WalkingType : dtl::type::size {
	empty,
	direction0,
	direction4,
	direction8
};

enum class IntelligenceType : dtl::type::size {
	empty,
	player,
	enemy
};

enum class BehaviorType : dtl::type::size {
	lower_middle,
	lower_left,
	middle_left,
	lower_right,
	middle_right,
	upper_left,
	upper_middle,
	upper_right,
	middle,
	empty
};

enum class DirectionType : dtl::type::size {
	lower_middle,
	lower_left,
	middle_left,
	lower_right,
	middle_right,
	upper_left,
	upper_middle,
	upper_right,
	middle
};

template<typename Int_, typename Float_>
struct Agent {
	
	//基本的な情報
	
	AgentType agent_type{ AgentType::empty }; //エージェントの種類
	WalkingType walking_type{ WalkingType::empty }; //歩行の種類
	IntelligenceType intelligence_type{ IntelligenceType::empty }; //知能の種類

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
	BehaviorType status{ BehaviorType::empty }; //動作の状態
	DirectionType direction{ DirectionType::lower_middle }; //向き
};
