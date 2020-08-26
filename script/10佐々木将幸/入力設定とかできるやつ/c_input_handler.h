#pragma once
#include"main.h"
#include"input.h"
#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include<vector>
#include"c_cmmond.h"
#include"e_input_list.h"

class CInput_Handler
{
private:
	//��΂ɌĂяo��
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

	//�ǉ��L�[��������Ă�����Ăяo��

	CCommand* leftctrl_key_up_arrow_press = nullptr;
	CCommand* leftctrl_key_left_arrow_press = nullptr;
	CCommand* leftctrl_key_down_arrow_press = nullptr;
	CCommand* leftctrl_key_right_arrow_press = nullptr;

	//�ǉ��L�[��������Ă��Ȃ��ꍇ�͌Ăяo��

	CCommand* not_leftctrl_key_up_arrow_press = nullptr;
	CCommand* not_leftctrl_key_left_arrow_press = nullptr;
	CCommand* not_leftctrl_key_down_arrow_press = nullptr;
	CCommand* not_leftctrl_key_right_arrow_press = nullptr;

	std::vector<CCommand*> com_vec;
public:
	//���͎擾���擾���A���s�Ȃǂ��I�������Ăяo���Ă�������
	void Input_Clean_Up();

	//���s�̂�
	void Execute(CBase& actor);

	//���s�̍Ō�ɃN���A
	void Execute_Clean_Up(CBase& actor);

	//���͎擾
	void Get_Input();

	//�L�[�ɃR�}���h�����蓖�Ă�
	void Set_Command(E_INPUT_LIST set_key, CCommand* in_command);

//�ÓI�֐�
//-------------------------------------------------------------------------------
	//���ڒ��̊֐����Ă�ł������ł��B�Ƃ肠�����K�v�ȏ�����Z�߂�����

	//���̃N���X���ǂ����ň�x�����ł��g���ꍇ��ԍŏ��ɌĂяo��
	static void Initialize();

	//���̃N���X���ǂ����ň�x�����ł��g������Ăяo��
	static void Finalize();

	//���̃N���X���ǂ����ň�x�����ł��g������S�̂�update�̍Ō�Ɏg��
	static void Update();



};