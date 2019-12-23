//DTL v0.4.14.0
#include <DTL/Double/AutoTileLoop.hpp>
#include <DTL/Double/Copy.hpp>
#include <DTL/Type/SizeT.hpp>
#include <DTL/Shape/RandomRect.hpp>
#include <DTL/Shape/Rect.hpp>
#include <DTL/Shape/RogueLike.hpp>
#include <DTL/Utility/ReplaceAll.hpp>
#include <DTL/Utility/ReplaceSome.hpp>
#include <DTL/Retouch/Bucket.hpp>
#include <DTL/Utility/Camera.hpp>

#include <Siv3D.hpp>

#include "Agent.hpp"
#include "Behavior.hpp"
#include "Flag.hpp"

constexpr int32 window_w{ 800 }; //ウィンドウ横サイズ
constexpr int32 window_h{ 600 }; //ウィンドウ縦サイズ

constexpr dtl::type::size mapchip_width{ 32 }; //マップ横サイズ
constexpr dtl::type::size mapchip_height{ 24 }; //マップ縦サイズ
constexpr dtl::type::size mapchip_pixel_size{ 80 }; //マップチップ1マス分のピクセルサイズ

template<typename Array_>
class AutoTile {
public:
	Array_ tile_image{};
	dtl::type::size tile_type{};
	dtl::type::size x_num{};
	dtl::type::size y_num{};
	dtl::type::size move_num{};

	dtl::type::size move_count{};

	template<typename Array_Ptr_>
	AutoTile(Array_Ptr_&& tile_image_, const dtl::type::size tile_type_, const dtl::type::size, const dtl::type::size, const dtl::type::size)
		:tile_image(std::move(tile_image_)), tile_type(tile_type_), x_num(1), y_num(1), move_num(1) {}
};

Color makeTile1(const Color& name_) {
	Color ice = name_;
	return ice;
}

struct AutoTile4 {
	int32 at1{}, at2{}, at3{}, at4{};
	bool operator<(int32 at_) noexcept {
		return at1 < at_ || at2 < at_ || at3 < at_ || at4 < at_;
	}
};

struct TileID {
	dtl::type::size index{};
	bool can_pass{ false }; //通行できるか
	bool can_overtake{ false }; //斜め移動ですり抜けできるか

};

//ゲームループクラス
class MainFrame {

	Font font{30};
	Font scene_event_font{120};

	DrawFlag draw_flag{};
	EventFlag event_flag{};

	dtl::type::size next_floor_time{};

	//キーが押されているフレーム数を格納する
	std::array<std::int_fast32_t, 256> key_frame{ {} };
	std::bitset<256> up_key{};
	std::bitset<256> down_key{};

	dtl::type::size floor_count{ 1 };

	dtl::type::size max_floor{ 1 };

	std::array<TileID, 8> tile_id{
	TileID{0,false,false}, //0 水床
	TileID{1,false,true}, //1 壁
	TileID{2,true,true}, //2 なし
	TileID{2,false,true}, //3 なし
	TileID{2,false,true}, //4 なし
	TileID{3,true,true}, //5 階段
	TileID{6,true,true}, //6 食料
	TileID{7,true,true} //6 食料
	};

	std::array<AutoTile<Color>, 8> ata { {
	AutoTile<Color>(makeTile1(Color(130,100,75)), 1, 2, 10, 1),
	AutoTile<Color>(makeTile1(Color(130,160,200)), 1, 4, 10, 2),
	AutoTile<Color>(makeTile1(Color(0,0,0)), 0, 0, 0, 0),
	AutoTile<Color>(makeTile1(Color(120,120,120)), 2, 1, 1, 1),
	AutoTile<Color>(makeTile1(Color(200,170,130)), 2, 1, 1, 1),
	AutoTile<Color>(makeTile1(Color(40,40,40)), 5, 6, 4, 3),
	AutoTile<Color>(makeTile1(Color(130,200,150)), 8, 1, 1, 1),
	AutoTile<Color>(makeTile1(Color(170,200,130)), 8, 1, 1, 1)
	} };

	using shape_t = std::uint_fast8_t;

	Agent<int32, float> agent; //エージェント

	std::array<dtl::Vec2<int32>, 9> around_vec2{};
	std::array<shape_t, 9> around{};
	dtl::base::MatrixRange mr{ 0, 0, mapchip_width, mapchip_height };

	enum : shape_t {
		tile_id_water,
		tile_id_wall,
		tile_id_empty1,
		tile_id_empty2,
		tile_id_empty3,
		tile_id_next,
		tile_id_m1,
		tile_id_m2
	};

	std::array<std::array<shape_t, mapchip_width>, mapchip_height> matrix_{ {} }; //フロアの状態を格納する
	std::array<std::array<shape_t, mapchip_width>, mapchip_height> look_{}; //サブマップの状態を格納する
	std::array<std::array<AutoTile4, mapchip_width>, mapchip_height> at_mat_{ {} }; //フロアの形状を格納する
	std::array<std::array<shape_t, mapchip_width>, mapchip_height> item_{}; //フロアのアイテムを格納する

	//カメラ設定
	dtl::MatrixRange win{ 0, 0, static_cast<dtl::type::size>(window_w), static_cast<dtl::type::size>(window_h) }; //描画範囲
	dtl::MatrixVec2 wp{ mapchip_pixel_size, mapchip_pixel_size }; //マスのサイズ
	dtl::Vec2<float> mc{ mapchip_width / 2.0f, mapchip_height / 2.0f }; //視点位置
	dtl::Vec2<int32> wssp{}; //始点座標
	dtl::Vec2<int32> wsss{}; //始点位置

	int32 loop_time{ 20 };

	void Create() {
		dtl::Rect<shape_t>(0).draw(this->matrix_);
		dtl::shape::RogueLike<shape_t>(0, 1, 2, 3, 4, 60, dtl::base::MatrixRange(4, 4, 2, 2), dtl::base::MatrixRange(3, 3, 4, 4)).draw(this->matrix_); //ローグライク生成
		this->item_ = this->look_ = this->matrix_;
		dtl::ReplaceAll<shape_t>(2, 3).draw(this->look_);
		dtl::ReplaceSome<shape_t>(1, 5, 2).draw(this->matrix_);

		dtl::ReplaceSome<shape_t>(10, 6, 2).draw(this->item_);
		dtl::ReplaceSome<shape_t>(10, 7, 2).draw(this->item_);

		for (dtl::type::size i{}; i < this->matrix_.size(); ++i)
			for (dtl::type::size j{}; j < this->matrix_[i].size(); ++j)
				if (this->matrix_[i][j] == 2) {
					agent.position.x = static_cast<int32>(j);
					agent.position.y = static_cast<int32>(i);
				}
		mc.x = float(agent.position.x + 0.5f);
		mc.y = float(agent.position.y + 0.5f);

		if (this->look_[agent.position.y][agent.position.x] == 2) dtl::Bucket<shape_t>(10, agent.position.x, agent.position.y).draw(this->look_);
		else if (this->look_[agent.position.y][agent.position.x] >= 2) this->look_[agent.position.y][agent.position.x] = 10;

		for (dtl::type::size y{}; y < this->matrix_.size(); ++y)
			for (dtl::type::size x{}; x < this->matrix_[y].size(); ++x) {
				if (this->matrix_[y][x] == tile_id_wall) this->matrix_[y][x] = tile_id_water;
				if (this->matrix_[y][x] >= 2 && this->matrix_[y][x] <= 4) this->matrix_[y][x] = 2;
			}
		dtl::RandomRect<shape_t>(1, 0.6).drawOperator(this->matrix_, [](const shape_t value) {return value == 0; });
		dtl::double_matrix::AutoTileLoop<shape_t, AutoTile4>().draw(this->matrix_, this->at_mat_); //オートタイル情報を格納
	}

	void UpdateEnvironment() { //キーボード入力を更新
		if (KeyN.up()) this->draw_flag.sub_map = !this->draw_flag.sub_map;
		if (KeyM.up()) this->draw_flag.main_map = !this->draw_flag.main_map;
		if (KeyL.up()) this->draw_flag.tile_middle_leftine = !this->draw_flag.tile_middle_leftine;
		if (KeyH.up()) this->draw_flag.hp_bar = !this->draw_flag.hp_bar;
		if (Key1.pressed() && this->wp.x > 24)--this->wp; //カメラを遠くする
		if (Key2.pressed() && this->wp.x < 160)++this->wp; //カメラを近くする
		if (Key3.up()) dtl::Rect<shape_t>(10).drawOperator(this->look_, [](const shape_t value_) {return value_ >= 2; });
	}

	void UpdateAnime() { //画像素材のアニメーションを更新
		if (this->loop_time >= 20)
			for (auto&& atas : ata) {
				if (atas.move_num >= 2) ++atas.move_count;
				if (atas.move_num <= atas.move_count) atas.move_count = 0;
			}
	}

	void DrawHP_Bar() { //HPバーの描画
		Rect(0, 30, 400, 150).draw(Color(246, 246, 246));
		font(U"Player Lv.", agent.level, U" B", floor_count, U"F").draw(10, 40, Color(5, 5, 5));
		font(U"HP:", agent.hp, U"/", agent.max_hp, U"  Hunger:", agent.hunger, U"/", agent.max_hunger, U"").draw(10, 130, Color(5, 5, 5));
		int32 hp_w{ 350 };
		int32 hp_ww{ static_cast<int32>(hp_w * agent.hp / agent.max_hp) };

		Rect(20, 100, hp_ww, 20).draw((agent.hp > agent.max_hp / 2) ? Color(0, 220, 0) :
			(agent.hp > agent.max_hp / 4) ? Color(220, 220, 0) : Color(220, 0, 0));
		Rect(20 + hp_ww, 100, hp_w - hp_ww, 20).draw(Color(220, 220, 220));
	}

	void DrawSubMap() { //サブマップの描画
		int32 bai = 8;
		for (dtl::type::size y{}; y < this->matrix_.size(); ++y)
			for (dtl::type::size x{}; x < this->matrix_[y].size(); ++x)
				if (this->look_[y][x] >= 10) {
					if (this->matrix_[y][x] == 5) Rect(static_cast<int32>(x) * bai, static_cast<int32>(y) * bai + 200, bai, bai).draw(Color(50, 255, 50, 100));
					else Rect(static_cast<int32>(x) * bai, static_cast<int32>(y) * bai + 200, bai, bai).draw(Color(255, 255, 255, 100));
					if (this->item_[y][x] >= 6)
						Circle(static_cast<int32>(x) * bai + bai / 2, static_cast<int32>(y) * bai + bai / 2 + 200, bai / 2).draw(Color(50, 50, 255, 100));
				}
		Circle(agent.position.x * bai + bai / 2, agent.position.y * bai + bai / 2 + 200, bai / 2).draw(Color(255, 50, 50, 100));
	}

	void DrawMainMap() {
		dtl::utility::setCameraLoop<float, int32>(win, wp, mc, wssp, wsss);
		dtl::Vec2<int32> wssp2(wssp);
		//cameraLoop
		dtl::Vec2<int32> wsss2(wsss);
		while (wsss2.x < 0) wsss2.x += static_cast<int32>(this->matrix_.front().size());
		while (wsss2.y < 0) wsss2.y += static_cast<int32>(this->matrix_.size());
		wsss2.x %= static_cast<int32>(this->matrix_.front().size());
		wsss2.y %= static_cast<int32>(this->matrix_.size());

		for (dtl::type::size y{ static_cast<dtl::type::size>(wsss2.y) };; ++y) {
			y %= static_cast<int32>(this->matrix_.size());
			wssp2.x = wssp.x;
			for (dtl::type::size x{ static_cast<dtl::type::size>(wsss2.x) };; ++x) {
				x %= static_cast<int32>(this->matrix_[y].size());

				int32 wsspy = wssp2.y + static_cast<int32>(win.y);

				if (ata[tile_id[this->matrix_[y][x]].index].tile_type == 2 || ata[tile_id[this->matrix_[y][x]].index].tile_type == 0
					|| (ata[tile_id[this->matrix_[y][x]].index].tile_type == 1
						&& this->at_mat_[y][x] < 4
						))
					Rect(wssp2.x, wsspy, static_cast<int32>(wp.x), static_cast<int32>(wp.y)).draw(ata[4].tile_image);

				if (ata[tile_id[this->matrix_[y][x]].index].tile_type == 1) {
					Rect(wssp2.x, wsspy, static_cast<int32>(wp.x), static_cast<int32>(wp.y)).draw(ata[tile_id[this->matrix_[y][x]].index].tile_image);
				}
				else if (ata[tile_id[this->matrix_[y][x]].index].tile_type == 2)
					Rect(wssp2.x, wsspy, static_cast<int32>(wp.x), static_cast<int32>(wp.y)).draw(ata[tile_id[this->matrix_[y][x]].index].tile_image);
				if (this->item_[y][x] >= 6) {
					Rect(wssp2.x + static_cast<int32>(wp.x) / 4, wsspy + static_cast<int32>(wp.x) / 4,
						static_cast<int32>(wp.x) / 2, static_cast<int32>(wp.y) / 2).draw(ata[tile_id[this->item_[y][x]].index].tile_image);
				}

				//補助タイルの描画
				if (draw_flag.tile_middle_leftine)
					Rect(wssp2.x, wsspy, static_cast<int32>(wp.x), static_cast<int32>(wp.y)).drawFrame(1, 1, Color(158, 53, 69));

				wssp2.x += static_cast<int32>(wp.x);
				if (wssp2.x >= static_cast<int32>(win.w)) break;
			}
			wssp2.y += static_cast<int32>(wp.y);
			if (wssp2.y >= static_cast<int32>(win.h)) break;
		}
		Rect(static_cast<int32>(win.w) / 2 - static_cast<int32>(wp.x) / 2, static_cast<int32>(win.h) / 2 - static_cast<int32>(wp.y) / 2, static_cast<int32>(wp.x), static_cast<int32>(wp.y)).draw(ata[5].tile_image);
	}

	void Draw() {
		if (draw_flag.main_map) this->DrawMainMap(); //メインマップの描画
		if (draw_flag.hp_bar) this->DrawHP_Bar(); //HPバーの描画
		if (draw_flag.sub_map) this->DrawSubMap(); //サブマップの描画
	}

	void EventFinishedWalking() {
		UpdateAgentHunger(agent);
		if (this->next_floor_time == 0)
			this->next_floor_time = (this->matrix_[agent.position.y][agent.position.x] == tile_id_next
				&& (KeyW.pressed() == 0)
				&& (KeyA.pressed() == 0)
				&& (KeyS.pressed() == 0)
				&& (KeyD.pressed() == 0)
				&& (KeyUp.pressed() == 0)
				&& (KeyLeft.pressed() == 0)
				&& (KeyDown.pressed() == 0)
				&& (KeyRight.pressed() == 0)) ? 4 : 0;
		if (this->next_floor_time >= 1) this->event_flag.is_next_floor_time = true;

		if (this->look_[agent.position.y][agent.position.x] == 2) dtl::Bucket<shape_t>(10, agent.position.x, agent.position.y).draw(this->look_);
		else if (this->look_[agent.position.y][agent.position.x] >= 2) this->look_[agent.position.y][agent.position.x] = 10;

		switch (this->item_[agent.position.y][agent.position.x]) {
		case 6:
			agent.hp += 5;
			if (agent.hp > agent.max_hp) agent.hp = agent.max_hp;
			break;
		case 7:
			agent.hunger += 5;
			if (agent.hunger > agent.max_hunger) agent.hunger = agent.max_hunger;
			break;
		}
		this->item_[agent.position.y][agent.position.x] = 3;
	}

	void EventNextFloor() {
		this->Create();
		++this->floor_count;
	}

public:

	void Init() { //ゲームを初期化
		dtl::utility::setCameraLoop<float, int32>(this->win, this->wp, this->mc, this->wssp, this->wsss);
		this->Create();
	}

	void Loop() { //メインループ処理

		this->event_flag.init(); //イベントフラグの初期化
		if (this->next_floor_time == 0) this->UpdateEnvironment(); //入力をもとに環境を更新
		this->UpdateAnime(); //画像素材のアニメーションを更新

		if (this->next_floor_time == 256 && this->event_flag.is_next_floor_time) {
			this->event_flag.next_floor = true;
		}
		else if (this->next_floor_time == 256 && this->event_flag.is_die_time) {
			agent = Agent<int32, float>{};
			this->Create();
			this->floor_count = 1;
		}

		if (this->next_floor_time >= 500) {
			this->next_floor_time = 0;
			this->event_flag.is_next_floor_time = false;
			this->event_flag.is_die_time = false;
		}

		AgentBehaviorAroundMatrix(dtl::utility::makeWrapper<shape_t>(this->matrix_), around_vec2, around, agent.position, mr);
		if (this->next_floor_time == 0) {
			UpdateAgentBehavior(around, tile_id, agent,
				(((KeyUp.pressed()) ? 1 : 0) + ((KeyW.pressed()) ? 1 : 0)),
				(((KeyDown.pressed()) ? 1 : 0) + ((KeyS.pressed()) ? 1 : 0)),
				(((KeyLeft.pressed()) ? 1 : 0) + ((KeyA.pressed()) ? 1 : 0)),
				(((KeyRight.pressed()) ? 1 : 0) + ((KeyD.pressed()) ? 1 : 0)));
		}
		else {
			UpdateAgentBehavior(around, tile_id, agent, 0, 0, 0, 0);
		}

		this->event_flag.finished_walking = UpdateAgentMove(this->around_vec2, this->agent); //移動後処理
		UpdateAgentStatus(this->agent, this->mc); //移動後カメラ処理

		if (this->event_flag.finished_walking) this->EventFinishedWalking(); //歩行終了処理

		event_flag.die = (agent.hp == 0); //死ぬ
		if (event_flag.die) {
			event_flag.is_die_time = true;
			if (this->next_floor_time == 0) {
				this->next_floor_time = 4;
				max_floor = this->floor_count;
			}
		}

		if (this->event_flag.next_floor) this->EventNextFloor(); //階段移動処理



		this->Draw(); //画面を描画する

		if (this->next_floor_time >= 1) {
			Rect(0, 0, window_w, window_h).draw(Color(0, 0, 0, ((this->next_floor_time >= 250) ?
				504 - static_cast<int32>(this->next_floor_time) : static_cast<int32>(this->next_floor_time))));

			if (this->event_flag.is_next_floor_time) {
				scene_event_font(U"B", floor_count + ((this->next_floor_time >= 256) ? 0 : 1), U"F").draw(window_w / 8, window_h / 8, Color(250, 250, 250));
			}
			if (this->event_flag.is_die_time) {
				scene_event_font(U"Game Over\nMax: B", max_floor, U"F").draw(window_w / 8, window_h / 8, Color(250, 250, 250));
			}
		}



		if (this->next_floor_time >= 1) {
			if (this->event_flag.is_next_floor_time) {
				this->next_floor_time += 4;
			}
			if (this->event_flag.is_die_time) {
				this->next_floor_time += 2;
			}
		}

		if (loop_time >= 20) loop_time = 0;
		else ++loop_time;
	}
};


void Main() {
	std::unique_ptr<MainFrame> main_frame(new(std::nothrow) MainFrame);
	if (main_frame) {
		main_frame->Init();
		while (System::Update()) main_frame->Loop();
	}
}