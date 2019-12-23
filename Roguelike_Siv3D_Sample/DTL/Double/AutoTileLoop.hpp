/*#######################################################################################
	Copyright (c) 2017-2019 Kasugaccho
	Copyright (c) 2018-2019 As Project
	https://github.com/Kasugaccho/DungeonTemplateLibrary
	wanotaitei@gmail.com

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#ifndef INCLUDED_DUNGEON_TEMPLATE_LIBRARY_DTL_DOUBLE_AUTO_TILE_LOOP_HPP
#define INCLUDED_DUNGEON_TEMPLATE_LIBRARY_DTL_DOUBLE_AUTO_TILE_LOOP_HPP

/*#######################################################################################
	日本語リファレンス (Reference-JP)
	https://github.com/Kasugaccho/DungeonTemplateLibrary/wiki/dtl::double_matrix::AutoTileLoop-(修正クラス)/
#######################################################################################*/

#include <DTL/Macros/constexpr.hpp>
#include <DTL/Type/SizeT.hpp>
#include <DTL/Range/RectBase.hpp>
#include <DTL/Utility/DrawJaggedDouble.hpp>

/*#######################################################################################
	[概要] "dtl名前空間"とは"DungeonTemplateLibrary"の全ての機能が含まれる名前空間である。
	[Summary] The "dtl" is a namespace that contains all the functions of "DungeonTemplateLibrary".
#######################################################################################*/
namespace dtl {
	inline namespace double_matrix { //"dtl::double_matrix"名前空間に属する

		//マスを指定した数値で埋める
		template<typename Matrix_Var1_, typename Matrix_Var2_>
		class AutoTileLoop : public ::dtl::range::RectBase<AutoTileLoop<Matrix_Var1_, Matrix_Var2_>>,
			public ::dtl::utility::DrawJaggedDouble<AutoTileLoop<Matrix_Var1_, Matrix_Var2_>, Matrix_Var1_, Matrix_Var2_> {
		private:


			///// エイリアス (Alias) /////

			using Index_Size = ::dtl::type::size;
			using ShapeBase_t = ::dtl::range::RectBase<AutoTileLoop>;
			using DrawBase_t = ::dtl::utility::DrawJaggedDouble<AutoTileLoop, Matrix_Var1_, Matrix_Var2_>;

			friend DrawBase_t;

			//Normal
			template<typename Matrix1_, typename Matrix2_, typename ...Args_>
			DTL_VERSIONING_CPP14_CONSTEXPR
			bool drawNormal(Matrix1_&& matrix1_, Matrix2_&& matrix2_, Args_&& ... args_) const noexcept {

				const Index_Size end_x_{ this->calcEndX(matrix2_.getX()) };
				const Index_Size end_y_{ this->calcEndY(matrix2_.getY()) };
				for (Index_Size row{ this->start_y }; row < end_y_; ++row)
					for (Index_Size col{ this->start_x }; col < end_x_; ++col) {

						const bool x1{ (matrix1_.get(col, row) == matrix1_.get(((col <= this->start_x) ? (end_x_) : col) - 1, row)) ? true : false };
						const bool y1{ (matrix1_.get(col, row) == matrix1_.get(col, ((row <= this->start_y) ? (end_y_) : row) - 1)) ? true : false };

						const bool x2{ (matrix1_.get(col, row) == matrix1_.get((col >= (end_x_ - 1)) ? (this->start_x) : (col + 1), row)) ? true : false };
						const bool y2{ (matrix1_.get(col, row) == matrix1_.get(col, ((row <= this->start_y) ? (end_y_) : row) - 1)) ? true : false };

						const bool x3{ (matrix1_.get(col, row) == matrix1_.get(((col <= this->start_x) ? (end_x_) : col) - 1, row)) ? true : false };
						const bool y3{ (matrix1_.get(col, row) == matrix1_.get(col, (row >= (end_y_ - 1)) ? (this->start_y) : (row + 1))) ? true : false };

						const bool x4{ (matrix1_.get(col, row) == matrix1_.get((col >= (end_x_ - 1)) ? (this->start_x) : (col + 1), row)) ? true : false };
						const bool y4{ (matrix1_.get(col, row) == matrix1_.get(col, (row >= (end_y_ - 1)) ? (this->start_y) : (row + 1))) ? true : false };

						const Matrix_Var2_ tmp{ 
							(x1) ? ((y1) ? ((matrix1_.get(col, row) ==
							matrix1_.get(((col <= this->start_x) ? (end_x_) : col) - 1, ((row <= this->start_y) ? (end_y_) : row) - 1)) ? 4 : 3) : 2) : ((y1) ? 1 : 0) ,

						 (x2) ? ((y2) ? ((matrix1_.get(col, row) ==
							matrix1_.get((col >= (end_x_ - 1)) ? (this->start_x) : (col + 1), ((row <= this->start_y) ? (end_y_) : row) - 1)) ? 4 : 3) : 2) : ((y2) ? 1 : 0) ,

						 (x3) ? ((y3) ? ((matrix1_.get(col, row) ==
							matrix1_.get(((col <= this->start_x) ? (end_x_) : col) - 1, (row >= (end_y_ - 1)) ? (this->start_y) : (row + 1))) ? 4 : 3) : 2) : ((y3) ? 1 : 0) ,

						 (x4) ? ((y4) ? ((matrix1_.get(col, row) ==
							matrix1_.get((col >= (end_x_ - 1)) ? (this->start_x) : (col + 1), (row >= (end_y_ - 1)) ? (this->start_y) : (row + 1))) ? 4 : 3) : 2) : ((y4) ? 1 : 0) };

						matrix2_.set(col, row, tmp, args_...);

					}
				return true;
			}

		public:


			///// コンストラクタ (Constructor) /////

			using ShapeBase_t::ShapeBase_t;
		};
	}
}

#endif //Included Dungeon Template Library
