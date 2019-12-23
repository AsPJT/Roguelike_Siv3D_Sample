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
	
	//��{�I�ȏ��
	
	dtl::type::size agent_type{ agent_type_empty }; //�G�[�W�F���g�̎��
	dtl::type::size walking_type{ walking_type_empty }; //���s�̎��
	dtl::type::size intelligence_type{ intelligence_type_empty }; //�m�\�̎��

	//�̂̃X�e�[�^�X

	dtl::type::size level{ 1 }; //���x��
	dtl::type::size hp{ 30 }; //�̗�
	dtl::type::size max_hp{ 50 }; //�ő�̗�
	dtl::type::size hunger{ 50 }; //������Q�[�W
	dtl::type::size max_hunger{ 100 }; //�ő啠����Q�[�W

	dtl::type::size hunger_steps{}; //����������̃J�E���g
	dtl::type::size max_hunger_steps{ 3 }; //���������

	dtl::Vec2<Int_> position{}; //�ʒu
	Float_ move{}; //�ׂ��Ȉʒu
	dtl::type::size behavior_status{ behavior_empty }; //����̏��
	dtl::type::size direction{ direction_lower_middle }; //����
};