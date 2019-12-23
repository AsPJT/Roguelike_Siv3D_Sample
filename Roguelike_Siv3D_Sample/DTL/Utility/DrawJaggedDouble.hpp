/*#######################################################################################
	Copyright (c) 2017-2019 Kasugaccho
	Copyright (c) 2018-2019 As Project
	https://github.com/Kasugaccho/DungeonTemplateLibrary
	wanotaitei@gmail.com

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#ifndef INCLUDED_DUNGEON_TEMPLATE_LIBRARY_DTL_UTILITY_DRAW_JAGGED_DOUBLE_HPP
#define INCLUDED_DUNGEON_TEMPLATE_LIBRARY_DTL_UTILITY_DRAW_JAGGED_DOUBLE_HPP

///// DTL Only /////

#include <DTL/Macros/constexpr.hpp>
#include <DTL/Type/Forward.hpp>
#include <DTL/Type/SizeT.hpp>
#include <DTL/Utility/MatrixWrapper.hpp>

/*#######################################################################################
	[概要] "dtl名前空間"とは"DungeonTemplateLibrary"の全ての機能が含まれる名前空間である。
	[Summary] The "dtl" is a namespace that contains all the functions of "DungeonTemplateLibrary".
#######################################################################################*/
namespace dtl {
	inline namespace utility { //"dtl::utility"名前空間に属する

		template<typename Derived, typename Matrix_Var1_, typename Matrix_Var2_, typename Return_Var_ = bool>
		class DrawJaggedDouble {
		private:

			///// エイリアス (Alias) /////

			using Index_Size = ::dtl::type::size;


		public:

			///// 生成呼び出し (Drawing Function Call) /////

/*#######################################################################################
	[概要] Matrixに描画する。
	[戻り値] 戻り値の型は bool である。[ true : 描画に成功したことを示す / false : 描画に失敗したことを示す ]
	[Summary] Draw on Matrix.
	[Return value] The return type is bool. [ true: Drawing was successful. / false: Drawing failed. ]
#######################################################################################*/
			template<typename Matrix1_, typename Matrix2_, typename... Args>
			constexpr Return_Var_ draw(Matrix1_& matrix1_, Matrix2_& matrix2_, Args&& ... args) const noexcept {
				return static_cast<const Derived*>(this)->drawNormal(::dtl::utility::makeWrapper<Matrix_Var1_>(matrix1_, DTL_TYPE_FORWARD<Args>(args)...), ::dtl::utility::makeWrapper<Matrix_Var2_>(matrix2_, DTL_TYPE_FORWARD<Args>(args)...));
			}

			/*#######################################################################################
				[概要] Matrixに描画する。
				[戻り値] 戻り値の型は bool である。[ true : 描画に成功したことを示す / false : 描画に失敗したことを示す ]
				[Summary] Draw on Matrix.
				[Return value] The return type is bool. [ true: Drawing was successful. / false: Drawing failed. ]
			#######################################################################################*/
			template<typename Matrix1_, typename Matrix2_, typename... Args>
			constexpr Return_Var_ drawAdd(Matrix1_& matrix1_, Matrix2_& matrix2_, Args&& ... args) const noexcept {
				return static_cast<const Derived*>(this)->drawNormal(::dtl::utility::makeWrapper<Matrix_Var1_>(matrix1_, DTL_TYPE_FORWARD<Args>(args)...), ::dtl::utility::makeWrapper<Matrix_Var2_>(matrix2_, DTL_TYPE_FORWARD<Args>(args)...), 0, 0, 0, 0);
			}

			/*#######################################################################################
				[概要] Matrixに描画する。
				[戻り値] 戻り値の型は bool である。[ true : 描画に成功したことを示す / false : 描画に失敗したことを示す ]
				[Summary] Draw on Matrix.
				[Return value] The return type is bool. [ true: Drawing was successful. / false: Drawing failed. ]
			#######################################################################################*/
			template<typename Matrix1_, typename Matrix2_, typename Function_, typename... Args>
			constexpr Return_Var_ drawOperator(Matrix1_& matrix1_, Matrix2_& matrix2_, Function_&& function_, Args&& ... args) const noexcept {
				return static_cast<const Derived*>(this)->drawNormal(::dtl::utility::makeWrapper<Matrix_Var1_>(matrix1_, DTL_TYPE_FORWARD<Args>(args)...), ::dtl::utility::makeWrapper<Matrix_Var2_>(matrix2_, DTL_TYPE_FORWARD<Args>(args)...), function_);
			}

			/*#######################################################################################
							[概要] Matrixに描画する。
							[戻り値] 戻り値の型は bool である。[ true : 描画に成功したことを示す / false : 描画に失敗したことを示す ]
							[Summary] Draw on Matrix.
							[Return value] The return type is bool. [ true: Drawing was successful. / false: Drawing failed. ]
						#######################################################################################*/
			template<typename Matrix1_, typename Matrix2_, typename Function_, typename... Args>
			constexpr Return_Var_ drawFunction(Matrix1_& matrix1_, Matrix2_& matrix2_, Function_&& function_, Args&& ... args) const noexcept {
				return static_cast<const Derived*>(this)->drawNormal(::dtl::utility::makeWrapper<Matrix_Var1_>(matrix1_, DTL_TYPE_FORWARD<Args>(args)...), ::dtl::utility::makeWrapper<Matrix_Var2_>(matrix2_, DTL_TYPE_FORWARD<Args>(args)...), function_, 0, 0, 0, 0);
			}

			/*#######################################################################################
							[概要] Matrixに描画する。
							[戻り値] 戻り値の型は bool である。[ true : 描画に成功したことを示す / false : 描画に失敗したことを示す ]
							[Summary] Draw on Matrix.
							[Return value] The return type is bool. [ true: Drawing was successful. / false: Drawing failed. ]
						#######################################################################################*/
			template<typename Matrix1_, typename Matrix2_, typename Function_, typename... Args>
			constexpr Return_Var_ drawAddOperator(Matrix1_& matrix1_, Matrix2_& matrix2_, Function_&& function_, Args&& ... args) const noexcept {
				return static_cast<const Derived*>(this)->drawNormal(::dtl::utility::makeWrapper<Matrix_Var1_>(matrix1_, DTL_TYPE_FORWARD<Args>(args)...), ::dtl::utility::makeWrapper<Matrix_Var2_>(matrix2_, DTL_TYPE_FORWARD<Args>(args)...), function_, 0, 0, 0);
			}

			/*#######################################################################################
				[概要] Matrixに描画する。
				[戻り値] 戻り値の型は bool である。[ true : 描画に成功したことを示す / false : 描画に失敗したことを示す ]
				[Summary] Draw on Matrix.
				[Return value] The return type is bool. [ true: Drawing was successful. / false: Drawing failed. ]
			#######################################################################################*/
			template<typename Matrix1_, typename Matrix2_>
			constexpr Return_Var_ drawArray(Matrix1_& matrix1_, Matrix2_& matrix2_, const Index_Size max_x_, const Index_Size max_y_) const noexcept {
				return static_cast<const Derived*>(this)->drawNormal(::dtl::utility::makeWrapper<Matrix_Var1_>(matrix1_, max_x_, max_y_), ::dtl::utility::makeWrapper<Matrix_Var2_>(matrix2_, max_x_, max_y_));
			}

			///// 生成呼び出しファンクタ (Drawing Functor) /////

/*#######################################################################################
	[概要] Matrixに描画する。
	[戻り値] 戻り値の型は bool である。[ true : 描画に成功したことを示す / false : 描画に失敗したことを示す ]
	[Summary] Draw on Matrix.
	[Return value] The return type is bool. [ true: Drawing was successful. / false: Drawing failed. ]
#######################################################################################*/
			template<typename Matrix1_, typename Matrix2_, typename ...Args_>
			constexpr Return_Var_ operator()(Matrix1_& matrix1_, Matrix2_& matrix2_, Args_&& ... args_) const noexcept {
				return this->draw(matrix1_, matrix2_, DTL_TYPE_FORWARD<Args_>(args_)...);
			}

		};
	}
}

#endif //Included Dungeon Template Library
