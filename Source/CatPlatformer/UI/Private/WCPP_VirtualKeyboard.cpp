// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_VirtualKeyboard.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

UWCPP_VirtualKeyboard::UWCPP_VirtualKeyboard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  CloseKeyboardButton(nullptr),
	  WS_KeyboardLayout(nullptr),
	  ChangeLanguageButton(nullptr),
	  bIsEnglishLanguage(false),
	  CurrentCapsLockMode(ECapsLockMode::Lowercase),
	  CapsLockButton(nullptr),
	  SpecialSymbolsButton(nullptr),
	  Btn_Eng_Letter_Q(nullptr),
	  Btn_Eng_Letter_W(nullptr),
	  Btn_Eng_Letter_E(nullptr),
	  Btn_Eng_Letter_R(nullptr),
	  Btn_Eng_Letter_T(nullptr),
	  Btn_Eng_Letter_Y(nullptr),
	  Btn_Eng_Letter_U(nullptr),
	  Btn_Eng_Letter_I(nullptr),
	  Btn_Eng_Letter_O(nullptr),
	  Btn_Eng_Letter_P(nullptr),
	  Btn_Eng_Letter_A(nullptr),
	  Btn_Eng_Letter_S(nullptr),
	  Btn_Eng_Letter_D(nullptr),
	  Btn_Eng_Letter_F(nullptr),
	  Btn_Eng_Letter_G(nullptr),
	  Btn_Eng_Letter_H(nullptr),
	  Btn_Eng_Letter_J(nullptr),
	  Btn_Eng_Letter_K(nullptr),
	  Btn_Eng_Letter_L(nullptr),
	  Btn_Eng_Letter_Z(nullptr),
	  Btn_Eng_Letter_X(nullptr),
	  Btn_Eng_Letter_C(nullptr),
	  Btn_Eng_Letter_V(nullptr),
	  Btn_Eng_Letter_B(nullptr),
	  Btn_Eng_Letter_N(nullptr),
	  Btn_Eng_Letter_M(nullptr),
	  TB_Eng_Letter_Q(nullptr),
	  TB_Eng_Letter_W(nullptr),
	  TB_Eng_Letter_E(nullptr),
	  TB_Eng_Letter_R(nullptr),
	  TB_Eng_Letter_T(nullptr),
	  TB_Eng_Letter_Y(nullptr),
	  TB_Eng_Letter_U(nullptr),
	  TB_Eng_Letter_I(nullptr),
	  TB_Eng_Letter_O(nullptr),
	  TB_Eng_Letter_P(nullptr),
	  TB_Eng_Letter_A(nullptr),
	  TB_Eng_Letter_S(nullptr),
	  TB_Eng_Letter_D(nullptr),
	  TB_Eng_Letter_F(nullptr),
	  TB_Eng_Letter_G(nullptr),
	  TB_Eng_Letter_H(nullptr),
	  TB_Eng_Letter_J(nullptr),
	  TB_Eng_Letter_K(nullptr),
	  TB_Eng_Letter_L(nullptr),
	  TB_Eng_Letter_Z(nullptr),
	  TB_Eng_Letter_X(nullptr),
	  TB_Eng_Letter_C(nullptr),
	  TB_Eng_Letter_V(nullptr),
	  TB_Eng_Letter_B(nullptr),
	  TB_Eng_Letter_N(nullptr),
	  TB_Eng_Letter_M(nullptr),
	  Btn_Ru_Letter_1(nullptr),
	  Btn_Ru_Letter_2(nullptr),
	  Btn_Ru_Letter_3(nullptr),
	  Btn_Ru_Letter_4(nullptr),
	  Btn_Ru_Letter_5(nullptr),
	  Btn_Ru_Letter_6(nullptr),
	  Btn_Ru_Letter_7(nullptr),
	  Btn_Ru_Letter_8(nullptr),
	  Btn_Ru_Letter_9(nullptr),
	  Btn_Ru_Letter_10(nullptr),
	  Btn_Ru_Letter_11(nullptr),
	  Btn_Ru_Letter_12(nullptr),
	  Btn_Ru_Letter_13(nullptr),
	  Btn_Ru_Letter_14(nullptr),
	  Btn_Ru_Letter_15(nullptr),
	  Btn_Ru_Letter_16(nullptr),
	  Btn_Ru_Letter_17(nullptr),
	  Btn_Ru_Letter_18(nullptr),
	  Btn_Ru_Letter_19(nullptr),
	  Btn_Ru_Letter_20(nullptr),
	  Btn_Ru_Letter_21(nullptr),
	  Btn_Ru_Letter_22(nullptr),
	  Btn_Ru_Letter_23(nullptr),
	  Btn_Ru_Letter_24(nullptr),
	  Btn_Ru_Letter_25(nullptr),
	  Btn_Ru_Letter_26(nullptr),
	  Btn_Ru_Letter_27(nullptr),
	  Btn_Ru_Letter_28(nullptr),
	  Btn_Ru_Letter_29(nullptr),
	  Btn_Ru_Letter_30(nullptr),
	  Btn_Ru_Letter_31(nullptr),
	  Btn_Ru_Letter_32(nullptr),
	  Btn_Ru_Letter_33(nullptr),
	  TB_Ru_Letter_1(nullptr),
	  TB_Ru_Letter_2(nullptr),
	  TB_Ru_Letter_3(nullptr),
	  TB_Ru_Letter_4(nullptr),
	  TB_Ru_Letter_5(nullptr),
	  TB_Ru_Letter_6(nullptr),
	  TB_Ru_Letter_7(nullptr),
	  TB_Ru_Letter_8(nullptr),
	  TB_Ru_Letter_9(nullptr),
	  TB_Ru_Letter_10(nullptr),
	  TB_Ru_Letter_11(nullptr),
	  TB_Ru_Letter_12(nullptr),
	  TB_Ru_Letter_13(nullptr),
	  TB_Ru_Letter_14(nullptr),
	  TB_Ru_Letter_15(nullptr),
	  TB_Ru_Letter_16(nullptr),
	  TB_Ru_Letter_17(nullptr),
	  TB_Ru_Letter_18(nullptr),
	  TB_Ru_Letter_19(nullptr),
	  TB_Ru_Letter_20(nullptr),
	  TB_Ru_Letter_21(nullptr),
	  TB_Ru_Letter_22(nullptr),
	  TB_Ru_Letter_23(nullptr),
	  TB_Ru_Letter_24(nullptr),
	  TB_Ru_Letter_25(nullptr),
	  TB_Ru_Letter_26(nullptr),
	  TB_Ru_Letter_27(nullptr),
	  TB_Ru_Letter_28(nullptr),
	  TB_Ru_Letter_29(nullptr),
	  TB_Ru_Letter_30(nullptr),
	  TB_Ru_Letter_31(nullptr),
	  TB_Ru_Letter_32(nullptr),
	  TB_Ru_Letter_33(nullptr),
	  Btn_Digit_1(nullptr),
	  Btn_Digit_2(nullptr),
	  Btn_Digit_3(nullptr),
	  Btn_Digit_4(nullptr),
	  Btn_Digit_5(nullptr),
	  Btn_Digit_6(nullptr),
	  Btn_Digit_7(nullptr),
	  Btn_Digit_8(nullptr),
	  Btn_Digit_9(nullptr),
	  Btn_Digit_0(nullptr),
	  TB_Digit_1(nullptr),
	  TB_Digit_2(nullptr),
	  TB_Digit_3(nullptr),
	  TB_Digit_4(nullptr),
	  TB_Digit_5(nullptr),
	  TB_Digit_6(nullptr),
	  TB_Digit_7(nullptr),
	  TB_Digit_8(nullptr),
	  TB_Digit_9(nullptr),
	  TB_Digit_0(nullptr),
	  Btn_Eng_Comma(nullptr),
	  Btn_Eng_Dot(nullptr),
	  Btn_Ru_Comma(nullptr),
	  Btn_Ru_Dot(nullptr),
	  Btn_Comma(nullptr),
	  Btn_Dot(nullptr),
	  Btn_At(nullptr),
	  Btn_NumberSign(nullptr),
	  Btn_Dollar(nullptr),
	  Btn_Percent(nullptr),
	  Btn_Caret(nullptr),
	  Btn_Ampersand(nullptr),
	  Btn_LeftParenthesis(nullptr),
	  Btn_RightParenthesis(nullptr),
	  Btn_Backslash(nullptr),
	  Btn_ForwardSlash(nullptr),
	  Btn_ExclamationMark(nullptr),
	  Btn_QuestionMark(nullptr),
	  Btn_Semicolon(nullptr),
	  Btn_Colon(nullptr),
	  Btn_LessThan(nullptr),
	  Btn_GreaterThan(nullptr),
	  Btn_Asterisk(nullptr),
	  Btn_Minus(nullptr),
	  Btn_Plus(nullptr),
	  Btn_Equals(nullptr),
	  Btn_Underscore(nullptr),
	  Btn_VerticalBar(nullptr),
	  Btn_QuotationMark(nullptr),
	  Btn_LeftQuotationMark(nullptr),
	  Btn_RightQuotationMark(nullptr),
	  Btn_LeftSquareBracket(nullptr),
	  Btn_RightSquareBracket(nullptr),
	  Btn_LeftCurlyBracket(nullptr),
	  Btn_RightCurlyBracket(nullptr),
	  Btn_Tilde(nullptr),
	  Btn_Apostrophe(nullptr),
	  Btn_Whitespace(nullptr),
	  Btn_Enter(nullptr),
	  Btn_Backspace(nullptr),
	  TB_Eng_Comma(nullptr),
	  TB_Eng_Dot(nullptr),
	  TB_Ru_Comma(nullptr),
	  TB_Ru_Dot(nullptr),
	  TB_Comma(nullptr),
	  TB_Dot(nullptr),
	  TB_At(nullptr),
	  TB_NumberSign(nullptr),
	  TB_Dollar(nullptr),
	  TB_Percent(nullptr),
	  TB_Caret(nullptr),
	  TB_Ampersand(nullptr),
	  TB_LeftParenthesis(nullptr),
	  TB_RightParenthesis(nullptr),
	  TB_Backslash(nullptr),
	  TB_ForwardSlash(nullptr),
	  TB_ExclamationMark(nullptr),
	  TB_QuestionMark(nullptr),
	  TB_Semicolon(nullptr),
	  TB_Colon(nullptr),
	  TB_LessThan(nullptr),
	  TB_GreaterThan(nullptr),
	  TB_Asterisk(nullptr),
	  TB_Minus(nullptr),
	  TB_Plus(nullptr),
	  TB_Equals(nullptr),
	  TB_Underscore(nullptr),
	  TB_VerticalBar(nullptr),
	  TB_QuotationMark(nullptr),
	  TB_LeftQuotationMark(nullptr),
	  TB_RightQuotationMark(nullptr),
	  TB_LeftSquareBracket(nullptr),
	  TB_RightSquareBracket(nullptr),
	  TB_LeftCurlyBracket(nullptr),
	  TB_RightCurlyBracket(nullptr),
	  TB_Tilde(nullptr),
	  TB_Apostrophe(nullptr),
	  TB_Whitespace(nullptr)
{
}

void UWCPP_VirtualKeyboard::NativeConstruct()
{
	Super::NativeConstruct();

	RussianLetters.Emplace(TB_Ru_Letter_1);
	RussianLetters.Emplace(TB_Ru_Letter_2);
	RussianLetters.Emplace(TB_Ru_Letter_3);
	RussianLetters.Emplace(TB_Ru_Letter_4);
	RussianLetters.Emplace(TB_Ru_Letter_5);
	RussianLetters.Emplace(TB_Ru_Letter_6);
	RussianLetters.Emplace(TB_Ru_Letter_7);
	RussianLetters.Emplace(TB_Ru_Letter_8);
	RussianLetters.Emplace(TB_Ru_Letter_9);
	RussianLetters.Emplace(TB_Ru_Letter_10);
	RussianLetters.Emplace(TB_Ru_Letter_11);
	RussianLetters.Emplace(TB_Ru_Letter_12);
	RussianLetters.Emplace(TB_Ru_Letter_13);
	RussianLetters.Emplace(TB_Ru_Letter_14);
	RussianLetters.Emplace(TB_Ru_Letter_15);
	RussianLetters.Emplace(TB_Ru_Letter_16);
	RussianLetters.Emplace(TB_Ru_Letter_17);
	RussianLetters.Emplace(TB_Ru_Letter_18);
	RussianLetters.Emplace(TB_Ru_Letter_19);
	RussianLetters.Emplace(TB_Ru_Letter_20);
	RussianLetters.Emplace(TB_Ru_Letter_21);
	RussianLetters.Emplace(TB_Ru_Letter_22);
	RussianLetters.Emplace(TB_Ru_Letter_23);
	RussianLetters.Emplace(TB_Ru_Letter_24);
	RussianLetters.Emplace(TB_Ru_Letter_25);
	RussianLetters.Emplace(TB_Ru_Letter_26);
	RussianLetters.Emplace(TB_Ru_Letter_27);
	RussianLetters.Emplace(TB_Ru_Letter_28);
	RussianLetters.Emplace(TB_Ru_Letter_29);
	RussianLetters.Emplace(TB_Ru_Letter_30);
	RussianLetters.Emplace(TB_Ru_Letter_31);
	RussianLetters.Emplace(TB_Ru_Letter_32);
	RussianLetters.Emplace(TB_Ru_Letter_33);

	EnglishLetters.Emplace(TB_Eng_Letter_Q);
	EnglishLetters.Emplace(TB_Eng_Letter_W);
	EnglishLetters.Emplace(TB_Eng_Letter_E);
	EnglishLetters.Emplace(TB_Eng_Letter_R);
	EnglishLetters.Emplace(TB_Eng_Letter_T);
	EnglishLetters.Emplace(TB_Eng_Letter_Y);
	EnglishLetters.Emplace(TB_Eng_Letter_U);
	EnglishLetters.Emplace(TB_Eng_Letter_I);
	EnglishLetters.Emplace(TB_Eng_Letter_O);
	EnglishLetters.Emplace(TB_Eng_Letter_P);
	EnglishLetters.Emplace(TB_Eng_Letter_A);
	EnglishLetters.Emplace(TB_Eng_Letter_S);
	EnglishLetters.Emplace(TB_Eng_Letter_D);
	EnglishLetters.Emplace(TB_Eng_Letter_F);
	EnglishLetters.Emplace(TB_Eng_Letter_G);
	EnglishLetters.Emplace(TB_Eng_Letter_H);
	EnglishLetters.Emplace(TB_Eng_Letter_J);
	EnglishLetters.Emplace(TB_Eng_Letter_K);
	EnglishLetters.Emplace(TB_Eng_Letter_L);
	EnglishLetters.Emplace(TB_Eng_Letter_Z);
	EnglishLetters.Emplace(TB_Eng_Letter_X);
	EnglishLetters.Emplace(TB_Eng_Letter_C);
	EnglishLetters.Emplace(TB_Eng_Letter_V);
	EnglishLetters.Emplace(TB_Eng_Letter_B);
	EnglishLetters.Emplace(TB_Eng_Letter_N);
	EnglishLetters.Emplace(TB_Eng_Letter_M);

	ChangeLanguageButton->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::ChangeLanguageButtonOnClick);
	CapsLockButton->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::CapsLockButtonOnClick);
	SpecialSymbolsButton->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::SpecialSymbolsButtonOnClick);
	CloseKeyboardButton->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::CloseKeyboardButtonOnClick);

	Btn_Eng_Letter_Q->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_Q_OnClick);
	Btn_Eng_Letter_W->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_W_OnClick);
	Btn_Eng_Letter_E->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_E_OnClick);
	Btn_Eng_Letter_R->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_R_OnClick);
	Btn_Eng_Letter_T->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_T_OnClick);
	Btn_Eng_Letter_Y->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_Y_OnClick);
	Btn_Eng_Letter_U->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_U_OnClick);
	Btn_Eng_Letter_I->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_I_OnClick);
	Btn_Eng_Letter_O->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_O_OnClick);
	Btn_Eng_Letter_P->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_P_OnClick);
	Btn_Eng_Letter_A->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_A_OnClick);
	Btn_Eng_Letter_S->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_S_OnClick);
	Btn_Eng_Letter_D->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_D_OnClick);
	Btn_Eng_Letter_F->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_F_OnClick);
	Btn_Eng_Letter_G->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_G_OnClick);
	Btn_Eng_Letter_H->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_H_OnClick);
	Btn_Eng_Letter_J->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_J_OnClick);
	Btn_Eng_Letter_K->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_K_OnClick);
	Btn_Eng_Letter_L->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_L_OnClick);
	Btn_Eng_Letter_Z->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_Z_OnClick);
	Btn_Eng_Letter_X->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_X_OnClick);
	Btn_Eng_Letter_C->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_C_OnClick);
	Btn_Eng_Letter_V->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_V_OnClick);
	Btn_Eng_Letter_B->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_B_OnClick);
	Btn_Eng_Letter_N->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_N_OnClick);
	Btn_Eng_Letter_M->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_M_OnClick);

	Btn_Ru_Letter_1->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_1_OnClick);
	Btn_Ru_Letter_2->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_2_OnClick);
	Btn_Ru_Letter_3->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_3_OnClick);
	Btn_Ru_Letter_4->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_4_OnClick);
	Btn_Ru_Letter_5->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_5_OnClick);
	Btn_Ru_Letter_6->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_6_OnClick);
	Btn_Ru_Letter_7->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_7_OnClick);
	Btn_Ru_Letter_8->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_8_OnClick);
	Btn_Ru_Letter_9->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_9_OnClick);
	Btn_Ru_Letter_10->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_10_OnClick);
	Btn_Ru_Letter_11->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_11_OnClick);
	Btn_Ru_Letter_12->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_12_OnClick);
	Btn_Ru_Letter_13->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_13_OnClick);
	Btn_Ru_Letter_14->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_14_OnClick);
	Btn_Ru_Letter_15->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_15_OnClick);
	Btn_Ru_Letter_16->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_16_OnClick);
	Btn_Ru_Letter_17->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_17_OnClick);
	Btn_Ru_Letter_18->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_18_OnClick);
	Btn_Ru_Letter_19->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_19_OnClick);
	Btn_Ru_Letter_20->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_20_OnClick);
	Btn_Ru_Letter_21->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_21_OnClick);
	Btn_Ru_Letter_22->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_22_OnClick);
	Btn_Ru_Letter_23->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_23_OnClick);
	Btn_Ru_Letter_24->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_24_OnClick);
	Btn_Ru_Letter_25->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_25_OnClick);
	Btn_Ru_Letter_26->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_26_OnClick);
	Btn_Ru_Letter_27->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_27_OnClick);
	Btn_Ru_Letter_28->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_28_OnClick);
	Btn_Ru_Letter_29->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_29_OnClick);
	Btn_Ru_Letter_30->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_30_OnClick);
	Btn_Ru_Letter_31->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_31_OnClick);
	Btn_Ru_Letter_32->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_32_OnClick);
	Btn_Ru_Letter_33->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_33_OnClick);

	Btn_Digit_1->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_1_OnClick);
	Btn_Digit_2->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_2_OnClick);
	Btn_Digit_3->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_3_OnClick);
	Btn_Digit_4->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_4_OnClick);
	Btn_Digit_5->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_5_OnClick);
	Btn_Digit_6->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_6_OnClick);
	Btn_Digit_7->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_7_OnClick);
	Btn_Digit_8->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_8_OnClick);
	Btn_Digit_9->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_9_OnClick);
	Btn_Digit_0->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_0_OnClick);

	Btn_Eng_Comma->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Comma_OnClick); // ,
	Btn_Ru_Comma->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Comma_OnClick); // ,
	Btn_Comma->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Comma_OnClick); // ,
	Btn_Eng_Dot->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dot_OnClick); // .
	Btn_Ru_Dot->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dot_OnClick); // .
	Btn_Dot->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dot_OnClick); // .

	Btn_At->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_At_OnClick); // @
	Btn_NumberSign->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_NumberSign_OnClick); // #
	Btn_Dollar->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dollar_OnClick); // $
	Btn_Percent->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Percent_OnClick); // %
	Btn_Caret->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Caret_OnClick); // ^
	Btn_Ampersand->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ampersand_OnClick); // &
	Btn_LeftParenthesis->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftParenthesis_OnClick); // (
	Btn_RightParenthesis->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightParenthesis_OnClick); // )
	Btn_Backslash->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Backslash_OnClick); /* \ */
	Btn_ForwardSlash->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_ForwardSlash_OnClick); /* / */
	Btn_ExclamationMark->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_ExclamationMark_OnClick); // !
	Btn_QuestionMark->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_QuestionMark_OnClick); // ?
	Btn_Semicolon->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Semicolon_OnClick); // ;
	Btn_Colon->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Colon_OnClick); // :
	Btn_LessThan->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_LessThan_OnClick); // <
	Btn_GreaterThan->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_GreaterThan_OnClick); // >
	Btn_Asterisk->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Asterisk_OnClick); // *
	Btn_Minus->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Minus_OnClick); // -
	Btn_Plus->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Plus_OnClick); // +
	Btn_Equals->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Equals_OnClick); // =
	Btn_Underscore->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Underscore_OnClick); // _
	Btn_VerticalBar->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_VerticalBar_OnClick); // |
	Btn_QuotationMark->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_QuotationMark_OnClick); // "
	Btn_LeftQuotationMark->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftQuotationMark_OnClick); // «
	Btn_RightQuotationMark->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightQuotationMark_OnClick); // »
	Btn_LeftSquareBracket->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftSquareBracket_OnClick); // [
	Btn_RightSquareBracket->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightSquareBracket_OnClick); // ] 
	Btn_LeftCurlyBracket->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftCurlyBracket_OnClick); // {
	Btn_RightCurlyBracket->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightCurlyBracket_OnClick); // }
	Btn_Tilde->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Tilde_OnClick); // ~
	Btn_Apostrophe->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Apostrophe_OnClick); // '
	Btn_Whitespace->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Whitespace_OnClick);

	Btn_Enter->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Enter_OnClick);
	Btn_Backspace->OnClicked.AddDynamic(this, &UWCPP_VirtualKeyboard::Btn_Backspace_OnClick);
}

void UWCPP_VirtualKeyboard::NativeDestruct()
{
	ChangeLanguageButton->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::ChangeLanguageButtonOnClick);
	CapsLockButton->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::CapsLockButtonOnClick);
	SpecialSymbolsButton->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::SpecialSymbolsButtonOnClick);
	CloseKeyboardButton->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::CloseKeyboardButtonOnClick);

	Btn_Eng_Letter_Q->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_Q_OnClick);
	Btn_Eng_Letter_W->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_W_OnClick);
	Btn_Eng_Letter_E->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_E_OnClick);
	Btn_Eng_Letter_R->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_R_OnClick);
	Btn_Eng_Letter_T->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_T_OnClick);
	Btn_Eng_Letter_Y->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_Y_OnClick);
	Btn_Eng_Letter_U->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_U_OnClick);
	Btn_Eng_Letter_I->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_I_OnClick);
	Btn_Eng_Letter_O->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_O_OnClick);
	Btn_Eng_Letter_P->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_P_OnClick);
	Btn_Eng_Letter_A->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_A_OnClick);
	Btn_Eng_Letter_S->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_S_OnClick);
	Btn_Eng_Letter_D->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_D_OnClick);
	Btn_Eng_Letter_F->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_F_OnClick);
	Btn_Eng_Letter_G->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_G_OnClick);
	Btn_Eng_Letter_H->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_H_OnClick);
	Btn_Eng_Letter_J->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_J_OnClick);
	Btn_Eng_Letter_K->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_K_OnClick);
	Btn_Eng_Letter_L->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_L_OnClick);
	Btn_Eng_Letter_Z->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_Z_OnClick);
	Btn_Eng_Letter_X->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_X_OnClick);
	Btn_Eng_Letter_C->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_C_OnClick);
	Btn_Eng_Letter_V->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_V_OnClick);
	Btn_Eng_Letter_B->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_B_OnClick);
	Btn_Eng_Letter_N->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_N_OnClick);
	Btn_Eng_Letter_M->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Eng_Letter_M_OnClick);

	Btn_Ru_Letter_1->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_1_OnClick);
	Btn_Ru_Letter_2->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_2_OnClick);
	Btn_Ru_Letter_3->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_3_OnClick);
	Btn_Ru_Letter_4->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_4_OnClick);
	Btn_Ru_Letter_5->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_5_OnClick);
	Btn_Ru_Letter_6->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_6_OnClick);
	Btn_Ru_Letter_7->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_7_OnClick);
	Btn_Ru_Letter_8->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_8_OnClick);
	Btn_Ru_Letter_9->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_9_OnClick);
	Btn_Ru_Letter_10->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_10_OnClick);
	Btn_Ru_Letter_11->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_11_OnClick);
	Btn_Ru_Letter_12->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_12_OnClick);
	Btn_Ru_Letter_13->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_13_OnClick);
	Btn_Ru_Letter_14->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_14_OnClick);
	Btn_Ru_Letter_15->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_15_OnClick);
	Btn_Ru_Letter_16->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_16_OnClick);
	Btn_Ru_Letter_17->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_17_OnClick);
	Btn_Ru_Letter_18->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_18_OnClick);
	Btn_Ru_Letter_19->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_19_OnClick);
	Btn_Ru_Letter_20->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_20_OnClick);
	Btn_Ru_Letter_21->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_21_OnClick);
	Btn_Ru_Letter_22->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_22_OnClick);
	Btn_Ru_Letter_23->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_23_OnClick);
	Btn_Ru_Letter_24->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_24_OnClick);
	Btn_Ru_Letter_25->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_25_OnClick);
	Btn_Ru_Letter_26->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_26_OnClick);
	Btn_Ru_Letter_27->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_27_OnClick);
	Btn_Ru_Letter_28->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_28_OnClick);
	Btn_Ru_Letter_29->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_29_OnClick);
	Btn_Ru_Letter_30->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_30_OnClick);
	Btn_Ru_Letter_31->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_31_OnClick);
	Btn_Ru_Letter_32->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_32_OnClick);
	Btn_Ru_Letter_33->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ru_Letter_33_OnClick);

	Btn_Digit_1->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_1_OnClick);
	Btn_Digit_2->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_2_OnClick);
	Btn_Digit_3->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_3_OnClick);
	Btn_Digit_4->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_4_OnClick);
	Btn_Digit_5->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_5_OnClick);
	Btn_Digit_6->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_6_OnClick);
	Btn_Digit_7->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_7_OnClick);
	Btn_Digit_8->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_8_OnClick);
	Btn_Digit_9->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_9_OnClick);
	Btn_Digit_0->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Digit_0_OnClick);

	Btn_Eng_Comma->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Comma_OnClick); // ,
	Btn_Ru_Comma->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Comma_OnClick); // ,
	Btn_Comma->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Comma_OnClick); // ,
	Btn_Eng_Dot->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dot_OnClick); // .
	Btn_Ru_Dot->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dot_OnClick); // .
	Btn_Dot->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dot_OnClick); // .

	Btn_At->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_At_OnClick); // @
	Btn_NumberSign->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_NumberSign_OnClick); // #
	Btn_Dollar->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Dollar_OnClick); // $
	Btn_Percent->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Percent_OnClick); // %
	Btn_Caret->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Caret_OnClick); // ^
	Btn_Ampersand->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Ampersand_OnClick); // &
	Btn_LeftParenthesis->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftParenthesis_OnClick); // (
	Btn_RightParenthesis->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightParenthesis_OnClick); // )
	Btn_Backslash->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Backslash_OnClick); /* \ */
	Btn_ForwardSlash->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_ForwardSlash_OnClick); /* / */
	Btn_ExclamationMark->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_ExclamationMark_OnClick); // !
	Btn_QuestionMark->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_QuestionMark_OnClick); // ?
	Btn_Semicolon->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Semicolon_OnClick); // ;
	Btn_Colon->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Colon_OnClick); // :
	Btn_LessThan->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_LessThan_OnClick); // <
	Btn_GreaterThan->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_GreaterThan_OnClick); // >
	Btn_Asterisk->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Asterisk_OnClick); // *
	Btn_Minus->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Minus_OnClick); // -
	Btn_Plus->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Plus_OnClick); // +
	Btn_Equals->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Equals_OnClick); // =
	Btn_Underscore->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Underscore_OnClick); // _
	Btn_VerticalBar->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_VerticalBar_OnClick); // |
	Btn_QuotationMark->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_QuotationMark_OnClick); // "
	Btn_LeftQuotationMark->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftQuotationMark_OnClick); // «
	Btn_RightQuotationMark->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightQuotationMark_OnClick); // »
	Btn_LeftSquareBracket->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftSquareBracket_OnClick); // [
	Btn_RightSquareBracket->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightSquareBracket_OnClick); // ] 
	Btn_LeftCurlyBracket->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_LeftCurlyBracket_OnClick); // {
	Btn_RightCurlyBracket->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_RightCurlyBracket_OnClick); // }
	Btn_Tilde->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Tilde_OnClick); // ~
	Btn_Apostrophe->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Apostrophe_OnClick); // '
	Btn_Whitespace->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Whitespace_OnClick);

	Btn_Enter->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Enter_OnClick);
	Btn_Backspace->OnClicked.RemoveDynamic(this, &UWCPP_VirtualKeyboard::Btn_Backspace_OnClick);

	Super::NativeDestruct();
}

void UWCPP_VirtualKeyboard::CloseKeyboardButtonOnClick()
{
	ClosingKeyboardDelegate.Broadcast();
}

void UWCPP_VirtualKeyboard::ChangeLanguageButtonOnClick()
{
	if (CurrentCapsLockMode == ECapsLockMode::UppercaseOneSymbol ||
		CurrentCapsLockMode == ECapsLockMode::Uppercase)
	{
		SetCurrentCapsLockMode(ECapsLockMode::Lowercase);
	}

	if (bIsEnglishLanguage)
	{
		WS_KeyboardLayout->SetActiveWidgetIndex(0);
	}
	else
	{
		WS_KeyboardLayout->SetActiveWidgetIndex(1);
	}
	bIsEnglishLanguage = !bIsEnglishLanguage;
}

void UWCPP_VirtualKeyboard::CapsLockButtonOnClick()
{
	switch (CurrentCapsLockMode)
	{
	case ECapsLockMode::Lowercase:
		SetCurrentCapsLockMode(ECapsLockMode::UppercaseOneSymbol);
		break;
	case ECapsLockMode::UppercaseOneSymbol:
		SetCurrentCapsLockMode(ECapsLockMode::Uppercase);
		break;
	case ECapsLockMode::Uppercase:
		SetCurrentCapsLockMode(ECapsLockMode::Lowercase);
		break;
	}
}

void UWCPP_VirtualKeyboard::SetCurrentCapsLockMode(const ECapsLockMode NewMode)
{
	if (CurrentCapsLockMode == NewMode)
		return;

	if (NewMode == ECapsLockMode::UppercaseOneSymbol || NewMode == ECapsLockMode::Uppercase)
	{
		MakeLettersUppercase(bIsEnglishLanguage ? EnglishLetters : RussianLetters);
	}
	else
	{
		MakeLettersLowercase(bIsEnglishLanguage ? EnglishLetters : RussianLetters);
	}
	CurrentCapsLockMode = NewMode;
	ChangeCapsLockImage(NewMode);
}

void UWCPP_VirtualKeyboard::MakeLettersUppercase(TSet<UTextBlock*>& LettersTextBlocks)
{
	for (const auto& LetterTextBlock : LettersTextBlocks)
	{
		LetterTextBlock->SetText(LetterTextBlock->GetText().ToUpper());
	}
}

void UWCPP_VirtualKeyboard::MakeLettersLowercase(TSet<UTextBlock*>& LettersTextBlocks)
{
	for (const auto& LetterTextBlock : LettersTextBlocks)
	{
		LetterTextBlock->SetText(LetterTextBlock->GetText().ToLower());
	}
}

void UWCPP_VirtualKeyboard::SpecialSymbolsButtonOnClick()
{
	if (WS_KeyboardLayout->GetActiveWidgetIndex() == 2)
	{
		WS_KeyboardLayout->SetActiveWidgetIndex(bIsEnglishLanguage ? 1 : 0);
	}
	else
	{
		WS_KeyboardLayout->SetActiveWidgetIndex(2);
	}
}

void UWCPP_VirtualKeyboard::TypeSymbol(const FText& TextToType)
{
	if (CurrentCapsLockMode == ECapsLockMode::UppercaseOneSymbol)
	{
		SetCurrentCapsLockMode(ECapsLockMode::Lowercase);
	}
	TypeSymbolDelegate.Broadcast(TextToType);
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_Q_OnClick()
{
	TypeSymbol(TB_Eng_Letter_Q->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_W_OnClick()
{
	TypeSymbol(TB_Eng_Letter_W->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_E_OnClick()
{
	TypeSymbol(TB_Eng_Letter_E->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_R_OnClick()
{
	TypeSymbol(TB_Eng_Letter_R->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_T_OnClick()
{
	TypeSymbol(TB_Eng_Letter_T->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_Y_OnClick()
{
	TypeSymbol(TB_Eng_Letter_Y->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_U_OnClick()
{
	TypeSymbol(TB_Eng_Letter_U->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_I_OnClick()
{
	TypeSymbol(TB_Eng_Letter_I->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_O_OnClick()
{
	TypeSymbol(TB_Eng_Letter_O->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_P_OnClick()
{
	TypeSymbol(TB_Eng_Letter_P->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_A_OnClick()
{
	TypeSymbol(TB_Eng_Letter_A->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_S_OnClick()
{
	TypeSymbol(TB_Eng_Letter_S->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_D_OnClick()
{
	TypeSymbol(TB_Eng_Letter_D->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_F_OnClick()
{
	TypeSymbol(TB_Eng_Letter_F->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_G_OnClick()
{
	TypeSymbol(TB_Eng_Letter_G->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_H_OnClick()
{
	TypeSymbol(TB_Eng_Letter_H->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_J_OnClick()
{
	TypeSymbol(TB_Eng_Letter_J->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_K_OnClick()
{
	TypeSymbol(TB_Eng_Letter_K->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_L_OnClick()
{
	TypeSymbol(TB_Eng_Letter_L->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_Z_OnClick()
{
	TypeSymbol(TB_Eng_Letter_Z->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_X_OnClick()
{
	TypeSymbol(TB_Eng_Letter_X->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_C_OnClick()
{
	TypeSymbol(TB_Eng_Letter_C->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_V_OnClick()
{
	TypeSymbol(TB_Eng_Letter_V->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_B_OnClick()
{
	TypeSymbol(TB_Eng_Letter_B->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_N_OnClick()
{
	TypeSymbol(TB_Eng_Letter_N->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Eng_Letter_M_OnClick()
{
	TypeSymbol(TB_Eng_Letter_M->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_1_OnClick()
{
	TypeSymbol(TB_Ru_Letter_1->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_2_OnClick()
{
	TypeSymbol(TB_Ru_Letter_2->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_3_OnClick()
{
	TypeSymbol(TB_Ru_Letter_3->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_4_OnClick()
{
	TypeSymbol(TB_Ru_Letter_4->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_5_OnClick()
{
	TypeSymbol(TB_Ru_Letter_5->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_6_OnClick()
{
	TypeSymbol(TB_Ru_Letter_6->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_7_OnClick()
{
	TypeSymbol(TB_Ru_Letter_7->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_8_OnClick()
{
	TypeSymbol(TB_Ru_Letter_8->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_9_OnClick()
{
	TypeSymbol(TB_Ru_Letter_9->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_10_OnClick()
{
	TypeSymbol(TB_Ru_Letter_10->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_11_OnClick()
{
	TypeSymbol(TB_Ru_Letter_11->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_12_OnClick()
{
	TypeSymbol(TB_Ru_Letter_12->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_13_OnClick()
{
	TypeSymbol(TB_Ru_Letter_13->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_14_OnClick()
{
	TypeSymbol(TB_Ru_Letter_14->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_15_OnClick()
{
	TypeSymbol(TB_Ru_Letter_15->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_16_OnClick()
{
	TypeSymbol(TB_Ru_Letter_16->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_17_OnClick()
{
	TypeSymbol(TB_Ru_Letter_17->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_18_OnClick()
{
	TypeSymbol(TB_Ru_Letter_18->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_19_OnClick()
{
	TypeSymbol(TB_Ru_Letter_19->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_20_OnClick()
{
	TypeSymbol(TB_Ru_Letter_20->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_21_OnClick()
{
	TypeSymbol(TB_Ru_Letter_21->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_22_OnClick()
{
	TypeSymbol(TB_Ru_Letter_22->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_23_OnClick()
{
	TypeSymbol(TB_Ru_Letter_23->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_24_OnClick()
{
	TypeSymbol(TB_Ru_Letter_24->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_25_OnClick()
{
	TypeSymbol(TB_Ru_Letter_25->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_26_OnClick()
{
	TypeSymbol(TB_Ru_Letter_26->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_27_OnClick()
{
	TypeSymbol(TB_Ru_Letter_27->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_28_OnClick()
{
	TypeSymbol(TB_Ru_Letter_28->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_29_OnClick()
{
	TypeSymbol(TB_Ru_Letter_29->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_30_OnClick()
{
	TypeSymbol(TB_Ru_Letter_30->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_31_OnClick()
{
	TypeSymbol(TB_Ru_Letter_31->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_32_OnClick()
{
	TypeSymbol(TB_Ru_Letter_32->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Ru_Letter_33_OnClick()
{
	TypeSymbol(TB_Ru_Letter_33->GetText());
}

void UWCPP_VirtualKeyboard::Btn_Digit_1_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("1")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_2_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("2")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_3_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("3")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_4_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("4")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_5_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("5")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_6_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("6")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_7_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("7")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_8_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("8")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_9_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("9")));
}

void UWCPP_VirtualKeyboard::Btn_Digit_0_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("0")));
}

void UWCPP_VirtualKeyboard::Btn_Dot_OnClick()
{
	TypeSymbol(FText::FromString(TEXT(".")));
}

void UWCPP_VirtualKeyboard::Btn_Comma_OnClick()
{
	TypeSymbol(FText::FromString(TEXT(",")));
}

void UWCPP_VirtualKeyboard::Btn_At_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("@")));
}

void UWCPP_VirtualKeyboard::Btn_NumberSign_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("#")));
}

void UWCPP_VirtualKeyboard::Btn_Dollar_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("$")));
}

void UWCPP_VirtualKeyboard::Btn_Percent_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("%")));
}

void UWCPP_VirtualKeyboard::Btn_Caret_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("^")));
}

void UWCPP_VirtualKeyboard::Btn_Ampersand_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("&")));
}

void UWCPP_VirtualKeyboard::Btn_LeftParenthesis_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("(")));
}

void UWCPP_VirtualKeyboard::Btn_RightParenthesis_OnClick()
{
	TypeSymbol(FText::FromString(TEXT(")")));
}

void UWCPP_VirtualKeyboard::Btn_Backslash_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("\\")));
}

void UWCPP_VirtualKeyboard::Btn_ForwardSlash_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("/")));
}

void UWCPP_VirtualKeyboard::Btn_ExclamationMark_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("!")));
}

void UWCPP_VirtualKeyboard::Btn_QuestionMark_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("?")));
}

void UWCPP_VirtualKeyboard::Btn_Semicolon_OnClick()
{
	TypeSymbol(FText::FromString(TEXT(";")));
}

void UWCPP_VirtualKeyboard::Btn_Colon_OnClick()
{
	TypeSymbol(FText::FromString(TEXT(":")));
}

void UWCPP_VirtualKeyboard::Btn_LessThan_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("<")));
}

void UWCPP_VirtualKeyboard::Btn_GreaterThan_OnClick()
{
	TypeSymbol(FText::FromString(TEXT(">")));
}

void UWCPP_VirtualKeyboard::Btn_Asterisk_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("*")));
}

void UWCPP_VirtualKeyboard::Btn_Minus_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("-")));
}

void UWCPP_VirtualKeyboard::Btn_Plus_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("+")));
}

void UWCPP_VirtualKeyboard::Btn_Equals_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("=")));
}

void UWCPP_VirtualKeyboard::Btn_Underscore_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("_")));
}

void UWCPP_VirtualKeyboard::Btn_VerticalBar_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("|")));
}

void UWCPP_VirtualKeyboard::Btn_QuotationMark_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("\"")));
}

void UWCPP_VirtualKeyboard::Btn_LeftQuotationMark_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("«")));
}

void UWCPP_VirtualKeyboard::Btn_RightQuotationMark_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("»")));
}

void UWCPP_VirtualKeyboard::Btn_LeftSquareBracket_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("[")));
}

void UWCPP_VirtualKeyboard::Btn_RightSquareBracket_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("]")));
}

void UWCPP_VirtualKeyboard::Btn_LeftCurlyBracket_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("{")));
}

void UWCPP_VirtualKeyboard::Btn_RightCurlyBracket_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("}")));
}

void UWCPP_VirtualKeyboard::Btn_Tilde_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("~")));
}

void UWCPP_VirtualKeyboard::Btn_Apostrophe_OnClick()
{
	TypeSymbol(FText::FromString(TEXT("\'")));
}

void UWCPP_VirtualKeyboard::Btn_Whitespace_OnClick()
{
	TypeSymbol(FText::FromString(TEXT(" ")));
}

void UWCPP_VirtualKeyboard::Btn_Enter_OnClick()
{
	EnterWasPressedDelegate.Broadcast();
}

void UWCPP_VirtualKeyboard::Btn_Backspace_OnClick()
{
	RemoveSymbolDelegate.Broadcast();
}
