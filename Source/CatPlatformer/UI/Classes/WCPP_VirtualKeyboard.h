// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class UTextBlock;
class UButton;
class UWidgetSwitcher;

#include "WCPP_VirtualKeyboard.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTypeSymbol, const FText& /* Symbol */);

DECLARE_MULTICAST_DELEGATE(FRemoveSymbol);

DECLARE_MULTICAST_DELEGATE(FEnterWasPressed)

DECLARE_MULTICAST_DELEGATE(FClosingKeyboard)

/** Enumeration for the current playing mode. */
UENUM(BlueprintType)
enum class ECapsLockMode : uint8
{
	Lowercase,
	UppercaseOneSymbol,
	Uppercase
};

/**
 * Parent widget class for virtual keyboard that can be used
 * during gamepad mode for typing something.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_VirtualKeyboard : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_VirtualKeyboard(const FObjectInitializer& ObjectInitializer);

	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;

	//=====================Delegates===============================
public:
	/** Delegate for passing symbol that was typed. */
	FTypeSymbol TypeSymbolDelegate;

	/**
	 * Delegate for notifying that user tries to delete
	 * the last typed symbol.
	 */
	FRemoveSymbol RemoveSymbolDelegate;

	/** Delegate for notifying that enter was pressed. */
	FEnterWasPressed EnterWasPressedDelegate;

	/**
	 * Delegate for notifying that user tries to close
	 * keyboard widget by pressing appropriate button on
	 * the virtual keyboard.
	 */
	FClosingKeyboard ClosingKeyboardDelegate;

	//==================Closing keyboard===========================
protected:
	/** Button for closing keyboard. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CloseKeyboardButton;

private:
	/** Function for closing keyboard. */
	UFUNCTION()
	void CloseKeyboardButtonOnClick();


	//==================Keyboard Layout============================
protected:
	/** Widget switcher for changing current keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* WS_KeyboardLayout;

	/** Button for changing language. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeLanguageButton;

private:
	/**
	 * Flag for indicating which language is used at the
	 * moment. True is russian language, false is english
	 * one.
	 */
	bool bIsEnglishLanguage;

	/** Array of text blocks containing russian letters. */
	UPROPERTY()
	TSet<UTextBlock*> RussianLetters;
	/** Array of text blocks containing english letters. */
	UPROPERTY()
	TSet<UTextBlock*> EnglishLetters;

	/** Function for changing current language. */
	UFUNCTION()
	void ChangeLanguageButtonOnClick();


	//=====================Caps Lock===============================
protected:
	/** Variable for current caps lock mode. */
	ECapsLockMode CurrentCapsLockMode;

	/** Button for making letters upper or lower case. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CapsLockButton;

private:
	/** Function for changing current Caps Lock mode. */
	UFUNCTION()
	void CapsLockButtonOnClick();

	/**
	 * Function for setting new value to the Caps Lock Mode
	 * variable.
	 */
	UFUNCTION()
	void SetCurrentCapsLockMode(ECapsLockMode NewMode);

	/**
	 * Function for making letters of the current language
	 * uppercase.
	 */
	UFUNCTION()
	static void MakeLettersUppercase(TSet<UTextBlock*>& LettersTextBlocks);

	/**
	 * Function for making letters of the current language
	 * lowercase.
	 */
	UFUNCTION()
	static void MakeLettersLowercase(TSet<UTextBlock*>& LettersTextBlocks);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangeCapsLockImage(ECapsLockMode NewMode);


	//===============Special Symbols Panel=========================

	/**
	 * Button for opening or closing the panel with special
	 * symbols.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SpecialSymbolsButton;

private:
	/**
	 * Function for opening or closing the panel with special
	 * symbols.
	 */
	UFUNCTION()
	void SpecialSymbolsButtonOnClick();


	//======================Typing=================================

	UFUNCTION()
	void TypeSymbol(const FText& TextToType);


	//==================English letters============================
protected:
	/** Buttons for english keyboard keys. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_Q;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_W;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_E;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_R;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_T;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_Y;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_U;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_I;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_O;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_P;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_A;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_S;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_D;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_F;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_G;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_H;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_J;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_K;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_L;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_Z;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_X;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_C;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_V;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_B;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_N;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Letter_M;

	/** Text blocks for english keyboard keys. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_Q;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_W;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_E;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_R;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_T;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_Y;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_U;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_I;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_O;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_P;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_A;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_S;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_D;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_F;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_G;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_H;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_J;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_K;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_L;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_Z;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_X;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_C;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_V;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_B;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_N;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Letter_M;

private:
	UFUNCTION()
	void Btn_Eng_Letter_Q_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_W_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_E_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_R_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_T_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_Y_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_U_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_I_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_O_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_P_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_A_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_S_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_D_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_F_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_G_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_H_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_J_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_K_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_L_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_Z_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_X_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_C_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_V_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_B_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_N_OnClick();
	UFUNCTION()
	void Btn_Eng_Letter_M_OnClick();


	//==================Russian letters============================
protected:
	/** Buttons for russian keyboard keys. */
	/** Button for russian letter Й. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_1;
	/** Button for russian letter Ц. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_2;
	/** Button for russian letter У. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_3;
	/** Button for russian letter К. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_4;
	/** Button for russian letter Е. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_5;
	/** Button for russian letter Н. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_6;
	/** Button for russian letter Г. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_7;
	/** Button for russian letter Ш. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_8;
	/** Button for russian letter Щ. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_9;
	/** Button for russian letter З. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_10;
	/** Button for russian letter Х. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_11;
	/** Button for russian letter Ъ. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_12;
	/** Button for russian letter Ф. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_13;
	/** Button for russian letter Ы. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_14;
	/** Button for russian letter В. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_15;
	/** Button for russian letter А. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_16;
	/** Button for russian letter П. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_17;
	/** Button for russian letter Р. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_18;
	/** Button for russian letter О. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_19;
	/** Button for russian letter Л. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_20;
	/** Button for russian letter Д. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_21;
	/** Button for russian letter Ж. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_22;
	/** Button for russian letter Э. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_23;
	/** Button for russian letter Я. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_24;
	/** Button for russian letter Ч. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_25;
	/** Button for russian letter С. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_26;
	/** Button for russian letter М. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_27;
	/** Button for russian letter И. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_28;
	/** Button for russian letter Т. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_29;
	/** Button for russian letter Ь. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_30;
	/** Button for russian letter Б. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_31;
	/** Button for russian letter Ю. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_32;
	/** Button for russian letter Ё. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Letter_33;

	/** Text blocks for russian keyboard keys. */
	/** Text block for russian letter Й. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_1;
	/** Text block for russian letter Ц. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_2;
	/** Text block for russian letter У. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_3;
	/** Text block for russian letter К. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_4;
	/** Text block for russian letter Е. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_5;
	/** Text block for russian letter Н. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_6;
	/** Text block for russian letter Г. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_7;
	/** Text block for russian letter Ш. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_8;
	/** Text block for russian letter Щ. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_9;
	/** Text block for russian letter З. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_10;
	/** Text block for russian letter Х. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_11;
	/** Text block for russian letter Ъ. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_12;
	/** Text block for russian letter Ф. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_13;
	/** Text block for russian letter Ы. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_14;
	/** Text block for russian letter В. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_15;
	/** Text block for russian letter А. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_16;
	/** Text block for russian letter П. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_17;
	/** Text block for russian letter Р. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_18;
	/** Text block for russian letter О. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_19;
	/** Text block for russian letter Л. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_20;
	/** Text block for russian letter Д. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_21;
	/** Text block for russian letter Ж. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_22;
	/** Text block for russian letter Э. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_23;
	/** Text block for russian letter Я. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_24;
	/** Text block for russian letter Ч. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_25;
	/** Text block for russian letter С. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_26;
	/** Text block for russian letter М. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_27;
	/** Text block for russian letter И. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_28;
	/** Text block for russian letter Т. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_29;
	/** Text block for russian letter Ь. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_30;
	/** Text block for russian letter Б. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_31;
	/** Text block for russian letter Ю. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_32;
	/** Text block for russian letter Ё. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Letter_33;

private:
	UFUNCTION()
	void Btn_Ru_Letter_1_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_2_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_3_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_4_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_5_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_6_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_7_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_8_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_9_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_10_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_11_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_12_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_13_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_14_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_15_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_16_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_17_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_18_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_19_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_20_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_21_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_22_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_23_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_24_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_25_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_26_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_27_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_28_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_29_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_30_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_31_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_32_OnClick();
	UFUNCTION()
	void Btn_Ru_Letter_33_OnClick();

	//======================Digits================================
public:
	/** Buttons for digits' keys. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_1;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_6;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_7;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_8;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_9;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Digit_0;

	/** Text blocks for digits' keys. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_6;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_7;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_8;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_9;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Digit_0;

private:
	UFUNCTION()
	void Btn_Digit_1_OnClick();
	UFUNCTION()
	void Btn_Digit_2_OnClick();
	UFUNCTION()
	void Btn_Digit_3_OnClick();
	UFUNCTION()
	void Btn_Digit_4_OnClick();
	UFUNCTION()
	void Btn_Digit_5_OnClick();
	UFUNCTION()
	void Btn_Digit_6_OnClick();
	UFUNCTION()
	void Btn_Digit_7_OnClick();
	UFUNCTION()
	void Btn_Digit_8_OnClick();
	UFUNCTION()
	void Btn_Digit_9_OnClick();
	UFUNCTION()
	void Btn_Digit_0_OnClick();


	//=================Special symbols============================
protected:
	/** Button for «,» symbol on english keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Comma;
	/** Button for «.» symbol on english keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Eng_Dot;
	/** Button for «,» symbol on russian keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Comma;
	/** Button for «.» symbol on russian keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ru_Dot;
	/** Button for «,» symbol on special symbols layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Comma;
	/** Button for «.» symbol on special symbols layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Dot;

	/** Button for «@» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_At;
	/** Button for «#» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_NumberSign;
	/** Button for «$» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Dollar;
	/** Button for «%» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Percent;
	/** Button for «^» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Caret;
	/** Button for «&» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Ampersand;
	/** Button for «(» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_LeftParenthesis;
	/** Button for «)» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_RightParenthesis;
	/** Button for «\» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Backslash;
	/** Button for «/» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_ForwardSlash;
	/** Button for «!» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_ExclamationMark;
	/** Button for «?» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_QuestionMark;
	/** Button for «;» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Semicolon;
	/** Button for «:» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Colon;
	/** Button for «<» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_LessThan;
	/** Button for «>» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_GreaterThan;
	/** Button for «*» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Asterisk;
	/** Button for «-» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Minus;
	/** Button for «+» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Plus;
	/** Button for «=» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Equals;
	/** Button for «_» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Underscore;
	/** Button for «|» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_VerticalBar;
	/**
	 * Button for «"» symbol.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_QuotationMark;
	/** Button for '«' symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_LeftQuotationMark;
	/** Button for '»' symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_RightQuotationMark;
	/** Button for «[» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_LeftSquareBracket;
	/** Button for «]» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_RightSquareBracket;
	/** Button for «{» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_LeftCurlyBracket;
	/** Button for «}» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_RightCurlyBracket;
	/** Button for «~» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Tilde;
	/** Button for «'» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Apostrophe;
	/** Button for « » symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Whitespace;

	/** Button for enter key. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Enter;
	/** Button for backspace key. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Backspace;


	/** Text blocks for special symbols. */

	/** Text block for «,» symbol on english keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Comma;
	/** Text block for «.» symbol on english keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Eng_Dot;
	/** Text block for «,» symbol on russian keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Comma;
	/** Text block for «.» symbol on russian keyboard layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ru_Dot;
	/** Text block for «,» symbol on special symbols layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Comma;
	/** Text block for «.» symbol on special symbols layout. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Dot;

	/** Text block for «@» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_At;
	/** Text block for «#» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_NumberSign;
	/** Text block for «$» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Dollar;
	/** Text block for «%» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Percent;
	/** Text block for «^» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Caret;
	/** Text block for «&» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Ampersand;
	/** Text block for «(» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_LeftParenthesis;
	/** Text block for «)» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_RightParenthesis;
	/** Text block for «\» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Backslash;
	/** Text block for «/» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_ForwardSlash;
	/** Text block for «!» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_ExclamationMark;
	/** Text block for «?» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_QuestionMark;
	/** Text block for «;» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Semicolon;
	/** Text block for «:» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Colon;
	/** Text block for «<» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_LessThan;
	/** Text block for «>» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_GreaterThan;
	/** Text block for «*» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Asterisk;
	/** Text block for «-» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Minus;
	/** Text block for «+» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Plus;
	/** Text block for «=» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Equals;
	/** Text block for «_» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Underscore;
	/** Text block for «|» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_VerticalBar;
	/** Text block for «"» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_QuotationMark;
	/** Text block for '«' symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_LeftQuotationMark;
	/** Text block for '»' symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_RightQuotationMark;
	/** Text block for «[» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_LeftSquareBracket;
	/** Text block for «]» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_RightSquareBracket;
	/** Text block for «{» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_LeftCurlyBracket;
	/** Text block for «}» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_RightCurlyBracket;
	/** Text block for «~» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Tilde;
	/** Text block for «'» symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Apostrophe;
	/** Text block for « » symbol. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Whitespace;

private:
	/** Function for typing «.» symbol. */
	UFUNCTION()
	void Btn_Dot_OnClick();
	/** Function for typing «,» symbol. */
	UFUNCTION()
	void Btn_Comma_OnClick();

	/** Function for typing «@» symbol. */
	UFUNCTION()
	void Btn_At_OnClick();
	/** Function for typing «#» symbol. */
	UFUNCTION()
	void Btn_NumberSign_OnClick();
	/** Function for typing «$» symbol. */
	UFUNCTION()
	void Btn_Dollar_OnClick();
	/** Function for typing «%» symbol. */
	UFUNCTION()
	void Btn_Percent_OnClick();
	/** Function for typing «^» symbol. */
	UFUNCTION()
	void Btn_Caret_OnClick();
	/** Function for typing «&» symbol. */
	UFUNCTION()
	void Btn_Ampersand_OnClick();
	/** Function for typing «(» symbol. */
	UFUNCTION()
	void Btn_LeftParenthesis_OnClick();
	/** Function for typing «)» symbol. */
	UFUNCTION()
	void Btn_RightParenthesis_OnClick();
	/** Function for typing «\» symbol. */
	UFUNCTION()
	void Btn_Backslash_OnClick();
	/** Function for typing «/» symbol. */
	UFUNCTION()
	void Btn_ForwardSlash_OnClick();
	/** Function for typing «!» symbol. */
	UFUNCTION()
	void Btn_ExclamationMark_OnClick();
	/** Function for typing «?» symbol. */
	UFUNCTION()
	void Btn_QuestionMark_OnClick();
	/** Function for typing «;» symbol. */
	UFUNCTION()
	void Btn_Semicolon_OnClick();
	/** Function for typing «:» symbol. */
	UFUNCTION()
	void Btn_Colon_OnClick();
	/** Function for typing «<» symbol. */
	UFUNCTION()
	void Btn_LessThan_OnClick();
	/** Function for typing «>» symbol. */
	UFUNCTION()
	void Btn_GreaterThan_OnClick();
	/** Function for typing «*» symbol. */
	UFUNCTION()
	void Btn_Asterisk_OnClick();
	/** Function for typing «-» symbol. */
	UFUNCTION()
	void Btn_Minus_OnClick();
	/** Function for typing «+» symbol. */
	UFUNCTION()
	void Btn_Plus_OnClick();
	/** Function for typing «=» symbol. */
	UFUNCTION()
	void Btn_Equals_OnClick();
	/** Function for typing «_» symbol. */
	UFUNCTION()
	void Btn_Underscore_OnClick();
	/** Function for typing «|» symbol. */
	UFUNCTION()
	void Btn_VerticalBar_OnClick();
	/**
	 * Function for typing «"» symbol.
	 */
	UFUNCTION()
	void Btn_QuotationMark_OnClick();
	/** Function for typing '«' symbol. */
	UFUNCTION()
	void Btn_LeftQuotationMark_OnClick();
	/** Function for typing '»' symbol. */
	UFUNCTION()
	void Btn_RightQuotationMark_OnClick();
	/** Function for typing «[» symbol. */
	UFUNCTION()
	void Btn_LeftSquareBracket_OnClick();
	/** Function for typing «]» symbol. */
	UFUNCTION()
	void Btn_RightSquareBracket_OnClick();
	/** Function for typing «{» symbol. */
	UFUNCTION()
	void Btn_LeftCurlyBracket_OnClick();
	/** Function for typing «}» symbol. */
	UFUNCTION()
	void Btn_RightCurlyBracket_OnClick();
	/** Function for typing «~» symbol. */
	UFUNCTION()
	void Btn_Tilde_OnClick();
	/** Function for typing «'» symbol. */
	UFUNCTION()
	void Btn_Apostrophe_OnClick();
	/** Function for typing « » symbol. */
	UFUNCTION()
	void Btn_Whitespace_OnClick();

	/** Function for pressing enter key. */
	UFUNCTION()
	void Btn_Enter_OnClick();
	/** Function for pressing backspace key. */
	UFUNCTION()
	void Btn_Backspace_OnClick();
};
