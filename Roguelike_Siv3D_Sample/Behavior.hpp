/*#######################################################################################
	Copyright (c) 2018-2022 As Project
	https://github.com/AsPJT/Roguelike_Siv3D_Sample
	wanotaitei@gmail.com

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#pragma once

template<typename Agent_>
bool UpdateAgentHunger(Agent_& agent) {
	if (agent.hunger == 0) {
		if (agent.hp == 0) return true;
		--agent.hp;
		return false;
	}
	++agent.hunger_steps;
	if (agent.hunger_steps >= agent.max_hunger_steps) {
		agent.hunger_steps = 0;
		--agent.hunger;
	}
	return false;
}

template<typename Array_, typename Int_>
void AgentBehaviorAround(Array_& array_, const dtl::base::Vec2<Int_>& vec2_, const dtl::base::MatrixRange& mr_) {
	const Int_ start_x{ static_cast<Int_>(((vec2_.x <= static_cast<Int_>(mr_.x)) ? static_cast<Int_>(mr_.x + mr_.w) : vec2_.x) - 1) };
	const Int_ start_y{ static_cast<Int_>(((vec2_.y <= static_cast<Int_>(mr_.y)) ? static_cast<Int_>(mr_.y + mr_.h) : vec2_.y) - 1)};
	const Int_ end_x{ static_cast<Int_>((vec2_.x >= static_cast<Int_>((mr_.x + mr_.w) - 1)) ? static_cast<Int_>(mr_.x) : (vec2_.x + 1))};
	const Int_ end_y{ static_cast<Int_>((vec2_.y >= static_cast<Int_>((mr_.y + mr_.h) - 1)) ? static_cast<Int_>(mr_.y) : (vec2_.y + 1)) };

	array_[size_t(DirectionType::lower_middle)] = dtl::base::Vec2<Int_>(vec2_.x, end_y);
	array_[size_t(DirectionType::lower_left)] = dtl::base::Vec2<Int_>(start_x, end_y);
	array_[size_t(DirectionType::middle_left)] = dtl::base::Vec2<Int_>(start_x, vec2_.y);
	array_[size_t(DirectionType::lower_right)] = dtl::base::Vec2<Int_>(end_x, end_y);
	array_[size_t(DirectionType::middle_right)] = dtl::base::Vec2<Int_>(end_x, vec2_.y);
	array_[size_t(DirectionType::upper_left)] = dtl::base::Vec2<Int_>(start_x, start_y);
	array_[size_t(DirectionType::upper_middle)] = dtl::base::Vec2<Int_>(vec2_.x, start_y);
	array_[size_t(DirectionType::upper_right)] = dtl::base::Vec2<Int_>(end_x, start_y);
	array_[size_t(DirectionType::middle)] = dtl::base::Vec2<Int_>(vec2_.x, vec2_.y);
}
template<typename Array_, typename Vec2Array_, typename Vec2_, typename Matrix_>
void AgentBehaviorAroundMatrix(Matrix_&& matrix_, Vec2Array_& vec2_array_, Array_& array_, const Vec2_ vec2_, const dtl::base::MatrixRange& mr_) {
	AgentBehaviorAround(vec2_array_, vec2_, mr_);
	for (dtl::type::size i{}; i < 9; ++i)
		array_[i] = matrix_.get(vec2_array_[i].x, vec2_array_[i].y);
}

template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorUpperLeft(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	agent.direction = DirectionType::upper_left;
	if (tile_id[array_[size_t(DirectionType::upper_left)]].can_pass
		&& tile_id[array_[size_t(DirectionType::upper_middle)]].can_overtake
		&& tile_id[array_[size_t(DirectionType::middle_left)]].can_overtake
		) {
		agent.status = BehaviorType::upper_left;
		return true;
	}
	return false;
}
template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorUpperRight(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	agent.direction = DirectionType::upper_right;
	if (tile_id[array_[size_t(DirectionType::upper_right)]].can_pass
		&& tile_id[array_[size_t(DirectionType::upper_middle)]].can_overtake
		&& tile_id[array_[size_t(DirectionType::middle_right)]].can_overtake
		) {
		agent.status = BehaviorType::upper_right;
		return true;
	}
	return false;
}
template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorUpperMiddle(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	if (tile_id[array_[size_t(DirectionType::upper_middle)]].can_pass) {
		agent.status = BehaviorType::upper_middle;
		return true;
	}
	return false;
}
template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorLowerLeft(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	agent.direction = DirectionType::lower_left;
	if (tile_id[array_[size_t(DirectionType::lower_left)]].can_pass
		&& tile_id[array_[size_t(DirectionType::lower_middle)]].can_overtake
		&& tile_id[array_[size_t(DirectionType::middle_left)]].can_overtake
		) {
		agent.status = BehaviorType::lower_left;
		return true;
	}
	return false;
}
template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorLowerRight(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	agent.direction = DirectionType::lower_right;
	if (tile_id[array_[size_t(DirectionType::lower_right)]].can_pass
		&& tile_id[array_[size_t(DirectionType::lower_middle)]].can_overtake
		&& tile_id[array_[size_t(DirectionType::middle_right)]].can_overtake
		) {
		agent.status = BehaviorType::lower_right;
		return true;
	}
	return false;
}
template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorLowerMiddle(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	if (tile_id[array_[size_t(DirectionType::lower_middle)]].can_pass) {
		agent.status = BehaviorType::lower_middle;
		return true;
	}
	return false;
}
template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorMiddleLeft(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	if (tile_id[array_[size_t(DirectionType::middle_left)]].can_pass) {
		agent.status = BehaviorType::middle_left;
		return true;
	}
	return false;
}
template<typename Array_, typename Tile_ID_, typename Agent_>
bool AgentBehaviorMiddleRight(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent) {
	if (tile_id[array_[size_t(DirectionType::middle_right)]].can_pass) {
		agent.status = BehaviorType::middle_right;
		return true;
	}
	return false;
}

template<typename Int_, typename Array_, typename Tile_ID_, typename Agent_>
void UpdateAgentBehavior(const Array_& array_, const Tile_ID_& tile_id, Agent_& agent, Int_ up_, Int_ down_, Int_ left_, Int_ right_) {
	if (agent.status != BehaviorType::empty) return; //動いていたら早期リターン
	if (left_ == 0) left_ = (std::numeric_limits<Int_>::max)();
	if (right_ == 0) right_ = (std::numeric_limits<Int_>::max)();
	if (up_ == 0) up_ = (std::numeric_limits<Int_>::max)();
	if (down_ == 0) down_ = (std::numeric_limits<Int_>::max)();

	if (up_ == down_) { //上下が押されていない時
		if (left_ == right_) return; //左右が押されていない時
		else if (left_ < right_) {
			agent.direction = DirectionType::middle_left;
			AgentBehaviorMiddleLeft(array_, tile_id, agent);
		}
		else {
			agent.direction = DirectionType::middle_right;
			AgentBehaviorMiddleRight(array_, tile_id, agent);
		}
		return;
	}
	//上下が押されている時
	if (left_ == right_) { //左右が押されていない時
		if (up_ < down_) {
			agent.direction = DirectionType::upper_middle;
			AgentBehaviorUpperMiddle(array_, tile_id, agent);
		}
		else {
			agent.direction = DirectionType::lower_middle;
			AgentBehaviorLowerMiddle(array_, tile_id, agent);
		}
		return;
	}
	//左右が押されている時
	if (up_ < down_) {
		if (left_ < right_) {
			if (!AgentBehaviorUpperLeft(array_, tile_id, agent)) {
				if (up_ == left_) return;
				else if (up_ < left_) {
					if (!AgentBehaviorUpperMiddle(array_, tile_id, agent)) AgentBehaviorMiddleLeft(array_, tile_id, agent);
				}
				else {
					if (!AgentBehaviorMiddleLeft(array_, tile_id, agent)) AgentBehaviorUpperMiddle(array_, tile_id, agent);
				}
			}
			return;
		}
		if (!AgentBehaviorUpperRight(array_, tile_id, agent)) {
			if (up_ == right_) return;
			else if (up_ < right_) {
				if (!AgentBehaviorUpperMiddle(array_, tile_id, agent)) AgentBehaviorMiddleRight(array_, tile_id, agent);
			}
			else {
				if (!AgentBehaviorMiddleRight(array_, tile_id, agent)) AgentBehaviorUpperMiddle(array_, tile_id, agent);
			}
		}
		return;
	}
	if (left_ < right_) {
		if (!AgentBehaviorLowerLeft(array_, tile_id, agent)) {
			if (down_ == left_) return;
			else if (down_ < left_) {
				if (!AgentBehaviorLowerMiddle(array_, tile_id, agent)) AgentBehaviorMiddleLeft(array_, tile_id, agent);
			}
			else {
				if (!AgentBehaviorMiddleLeft(array_, tile_id, agent)) AgentBehaviorLowerMiddle(array_, tile_id, agent);
			}
		}
		return;
	}
	if (!AgentBehaviorLowerRight(array_, tile_id, agent)) {
		if (down_ == right_) return;
		else if (down_ < right_) {
			if (!AgentBehaviorLowerMiddle(array_, tile_id, agent)) AgentBehaviorMiddleRight(array_, tile_id, agent);
		}
		else {
			if (!AgentBehaviorMiddleRight(array_, tile_id, agent)) AgentBehaviorLowerMiddle(array_, tile_id, agent);
		}
	}
	return;
}

template<typename Array_, typename Agent_>
bool UpdateAgentMove(Array_& array_vec2_, Agent_& agent) {
	if (agent.status == BehaviorType::empty) return false;
	agent.move += 0.1f;
	if (agent.move < 1.0f) return false;
	agent.move = 0.0f;
	agent.position = array_vec2_[size_t(agent.status)];
	agent.status = BehaviorType::empty;
	return true;
}

template<typename Agent_>
void UpdateAgentStatus(Agent_& agent, dtl::Vec2<float>& mc) {
	mc = dtl::Vec2<float>(agent.position.x + 0.5f, agent.position.y + 0.5f);
	switch (agent.status) {
	case BehaviorType::upper_middle:mc += dtl::Vec2<float>(0.0f, -agent.move); break;
	case BehaviorType::lower_middle:mc += dtl::Vec2<float>(0.0f, +agent.move); break;
	case BehaviorType::lower_right:mc += dtl::Vec2<float>(+agent.move, +agent.move); break;
	case BehaviorType::lower_left:mc += dtl::Vec2<float>(-agent.move, +agent.move); break;
	case BehaviorType::middle_left:mc += dtl::Vec2<float>(-agent.move, 0.0f); break;
	case BehaviorType::middle_right:mc += dtl::Vec2<float>(+agent.move, 0.0f); break;
	case BehaviorType::upper_right:mc += dtl::Vec2<float>(+agent.move, -agent.move); break;
	case BehaviorType::upper_left:mc += dtl::Vec2<float>(-agent.move, -agent.move); break;
	}
}
