#pragma once

//�`��t���O�Ǘ�
struct DrawFlag {
	bool main_map{ true };
	bool sub_map{ true };
	bool tile_middle_leftine{ false };
	bool hp_bar{ true };
};

//�C�x���g�t���O�Ǘ�
struct EventFlag {
	bool finished_walking{ false }; //1�}�X�ړ�������
	bool next_floor{ false }; //���̊K�֍s����

	bool die{ false }; //���񂾎�
	bool hungry{ false }; //���������E�̎�

	bool is_next_floor_time{ false };
	bool is_die_time{ false };

	void init() {
		finished_walking = false;
		next_floor = false;
		die = false;
		hungry = false;
	}
};