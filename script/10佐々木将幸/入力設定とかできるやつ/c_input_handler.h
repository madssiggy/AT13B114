#pragma once
#include"main.h"
#include"input.h"
#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>
#include<vector>
#include"c_cmmond.h"
#include"e_input_list.h"

class CInput_Handler
{
private:
	//絶対に呼び出す
	CCommand* key_a_press = nullptr;
	CCommand* key_s_press = nullptr;
	CCommand* key_w_press = nullptr;
	CCommand* key_d_press = nullptr;
	CCommand* key_q_press = nullptr;
	CCommand* key_e_press = nullptr;

	CCommand* key_space_press = nullptr;

	CCommand* key_up_arrow_press = nullptr;
	CCommand* key_left_arrow_press = nullptr;
	CCommand* key_down_arrow_press = nullptr;
	CCommand* key_right_arrow_press = nullptr;

	CCommand* key_1_trigger = nullptr;

	//追加キーが押されていたら呼び出す

	CCommand* leftctrl_key_up_arrow_press = nullptr;
	CCommand* leftctrl_key_left_arrow_press = nullptr;
	CCommand* leftctrl_key_down_arrow_press = nullptr;
	CCommand* leftctrl_key_right_arrow_press = nullptr;

	//追加キーが押されていない場合は呼び出す

	CCommand* not_leftctrl_key_up_arrow_press = nullptr;
	CCommand* not_leftctrl_key_left_arrow_press = nullptr;
	CCommand* not_leftctrl_key_down_arrow_press = nullptr;
	CCommand* not_leftctrl_key_right_arrow_press = nullptr;

	std::vector<CCommand*> com_vec;
public:
	//入力取得を取得し、実行などが終わったら呼び出してください
	void Input_Clean_Up();

	//実行のみ
	void Execute(CBase& actor);

	//実行の最後にクリア
	void Execute_Clean_Up(CBase& actor);

	//入力取得
	void Get_Input();

	//キーにコマンドを割り当てる
	void Set_Command(E_INPUT_LIST set_key, CCommand* in_command);

//静的関数
//-------------------------------------------------------------------------------
	//直接中の関数を呼んでもいいです。とりあえず必要な処理を纏めただけ

	//このクラスをどこかで一度だけでも使う場合一番最初に呼び出す
	static void Initialize();

	//このクラスをどこかで一度だけでも使ったら呼び出す
	static void Finalize();

	//このクラスをどこかで一度だけでも使ったら全体のupdateの最後に使う
	static void Update();



};