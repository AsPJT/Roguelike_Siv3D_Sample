/*#######################################################################################
	Copyright (c) 2018-2022 As Project
	https://github.com/AsPJT/Roguelike_Siv3D_Sample
	wanotaitei@gmail.com

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#pragma once


enum class AgentType : size_t {
	empty,
	player,
	enemy
};

enum class WalkingType : size_t {
	empty,
	direction0,
	direction4,
	direction8
};

enum class IntelligenceType : size_t {
	empty,
	player,
	enemy
};

enum class BehaviorType : size_t {
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

enum class DirectionType : size_t {
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

	AgentType agent_type = AgentType::empty; //エージェントの種類
	WalkingType walking_type = WalkingType::empty; //歩行の種類
	IntelligenceType intelligence_type = IntelligenceType::empty; //知能の種類

	//個体のステータス

	size_t level = 1; //レベル
	size_t hp = 30; //体力
	size_t max_hp = 50; //最大体力
	size_t hunger = 50; //腹減りゲージ
	size_t max_hunger = 100; //最大腹減りゲージ

	size_t hunger_steps = 0; //腹減り歩数のカウント
	size_t max_hunger_steps = 3; //腹減り歩数

	dtl::Vec2<Int_> position{}; //位置
	Float_ move{}; //細かな位置
	BehaviorType status = BehaviorType::empty; //動作の状態
	DirectionType direction = DirectionType::lower_middle; //向き
};
