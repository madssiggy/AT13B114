#include"input.h"
#include"c_cmmond.h"
#include"c_input_handler.h"

//���͎擾���擾���A���s�Ȃǂ��I�������Ăяo���Ă�������
void CInput_Handler::Input_Clean_Up()
{
	com_vec.clear();
}

//���s�̂�
void CInput_Handler::Execute(CBase& actor)
{
	for (auto i : com_vec)
	{
		if (i)
		{
			i->execute(actor);
		}
	}
}

//���s�̍Ō�ɃN���A
void CInput_Handler::Execute_Clean_Up(CBase& actor)
{
	Execute(actor);
	Input_Clean_Up();
}

//���͎擾
void CInput_Handler::Get_Input()
{

	//�e�L�[�������ꂽ���Ƃ�Ԃ�

	//�K���Ăяo��
	if (Keyboard_IsPress(DIK_A))
	{
		com_vec.push_back(key_a_press);
	}

	if (Keyboard_IsPress(DIK_S))
	{
		com_vec.push_back(key_s_press);
	}

	if (Keyboard_IsPress(DIK_W))
	{
		com_vec.push_back(key_w_press);
	}

	if (Keyboard_IsPress(DIK_D))
	{
		com_vec.push_back(key_d_press);
	}
	if (Keyboard_IsPress(DIK_Q))
	{
		com_vec.push_back(key_q_press);
	}

	if (Keyboard_IsPress(DIK_E))
	{
		com_vec.push_back(key_e_press);
	}

	if (Keyboard_IsPress(DIK_SPACE))
	{
		com_vec.push_back(key_space_press);
	}

	if (Keyboard_IsPress(DIK_UPARROW))
	{
		com_vec.push_back(key_up_arrow_press);
	}

	if (Keyboard_IsPress(DIK_LEFTARROW))
	{
		com_vec.push_back(key_left_arrow_press);
	}

	if (Keyboard_IsPress(DIK_DOWNARROW))
	{
		com_vec.push_back(key_down_arrow_press);
	}

	if (Keyboard_IsPress(DIK_RIGHTARROW))
	{
		com_vec.push_back(key_right_arrow_press);
	}


	if (Keyboard_IsTrigger(DIK_1))
	{
		com_vec.push_back(key_1_trigger);
	}

	//�ǉ��L�[�������ꂢ��ꍇ�̂݌Ăяo��
	if (Keyboard_IsPress(DIK_UPARROW) && Keyboard_IsPress(DIK_LCONTROL))
	{
		com_vec.push_back(leftctrl_key_up_arrow_press);
	}

	if (Keyboard_IsPress(DIK_LEFTARROW) && Keyboard_IsPress(DIK_LCONTROL))
	{
		com_vec.push_back(leftctrl_key_left_arrow_press);
	}

	if (Keyboard_IsPress(DIK_DOWNARROW) && Keyboard_IsPress(DIK_LCONTROL))
	{
		com_vec.push_back(leftctrl_key_down_arrow_press);
	}

	if (Keyboard_IsPress(DIK_RIGHTARROW) && Keyboard_IsPress(DIK_LCONTROL))
	{
		com_vec.push_back(leftctrl_key_right_arrow_press);
	}



	//�ǉ��L�[��������Ă��Ȃ��ꍇ�͌Ăяo��

	if (Keyboard_IsPress(DIK_UPARROW) && (!Keyboard_IsPress(DIK_LCONTROL)))
	{
		com_vec.push_back(not_leftctrl_key_up_arrow_press);
	}

	if (Keyboard_IsPress(DIK_LEFTARROW) && (!Keyboard_IsPress(DIK_LCONTROL)))
	{
		com_vec.push_back(not_leftctrl_key_left_arrow_press);
	}

	if (Keyboard_IsPress(DIK_DOWNARROW) && (!Keyboard_IsPress(DIK_LCONTROL)))
	{
		com_vec.push_back(not_leftctrl_key_down_arrow_press);
	}

	if (Keyboard_IsPress(DIK_RIGHTARROW) && (!Keyboard_IsPress(DIK_LCONTROL)))
	{
		com_vec.push_back(not_leftctrl_key_right_arrow_press);
	}
}

//�L�[�ɃR�}���h�����蓖�Ă�
void CInput_Handler::Set_Command(E_INPUT_LIST set_key, CCommand* in_command)
{
	switch (set_key)
	{

	case E_INPUT_LIST::KEY_A_PRESS:
		key_a_press = in_command;
		break;

	case E_INPUT_LIST::KEY_S_PRESS:
		key_s_press = in_command;
		break;

	case E_INPUT_LIST::KEY_W_PRESS:
		key_w_press = in_command;
		break;

	case E_INPUT_LIST::KEY_D_PRESS:
		key_d_press = in_command;
		break;

	case E_INPUT_LIST::KEY_Q_PRESS:
		key_q_press = in_command;
		break;

	case E_INPUT_LIST::KEY_E_PRESS:
		key_e_press = in_command;
		break;

	case E_INPUT_LIST::KEY_SPACE_PRESS:
		key_space_press = in_command;
		break;

	case E_INPUT_LIST::KEY_UP_ARROW_PRESS:
		key_up_arrow_press = in_command;
		break;

	case E_INPUT_LIST::KEY_LEFT_ARROW_PRESS:
		key_left_arrow_press = in_command;
		break;

	case E_INPUT_LIST::KEY_DOWN_ARROW_PRESS:
		key_down_arrow_press = in_command;
		break;

	case E_INPUT_LIST::KEY_RIGHT_ARROW_PRESS:
		key_right_arrow_press = in_command;
		break;

	case E_INPUT_LIST::KEY_1_TRIGGER:
		key_1_trigger = key_1_trigger;
		break;

		//�ǉ��L�[�������ꂢ��ꍇ�̂�

	case E_INPUT_LIST::LEFT_CTRL_KEY_UP_ARROW_PRESS:
		leftctrl_key_up_arrow_press = in_command;
		break;

	case E_INPUT_LIST::LEFT_CTRL_KEY_LEFT_ARROW_PRESS:
		leftctrl_key_left_arrow_press = in_command;
		break;

	case E_INPUT_LIST::LEFT_CTRL_KEY_DOWN_ARROW_PRESS:
		leftctrl_key_down_arrow_press = in_command;
		break;

	case E_INPUT_LIST::LEFT_CTRL_KEY_RIGHT_ARROW_PRESS:
		leftctrl_key_right_arrow_press = in_command;
		break;

		//�ǉ��L�[��������Ă��Ȃ��ꍇ�̂�
	case E_INPUT_LIST::NOT_LEFT_CTRL_KEY_UP_ARROW_PRESS:
		not_leftctrl_key_up_arrow_press = in_command;
		break;

	case E_INPUT_LIST::NOT_LEFT_CTRL_KEY_LEFT_ARROW_PRESS:
		not_leftctrl_key_left_arrow_press = in_command;
		break;

	case E_INPUT_LIST::NOT_LEFT_CTRL_KEY_DOWN_ARROW_PRESS:
		not_leftctrl_key_down_arrow_press = in_command;
		break;

	case E_INPUT_LIST::NOT_LEFT_CTRL_KEY_RIGHT_ARROW_PRESS:
		not_leftctrl_key_right_arrow_press = in_command;
		break;

	default:
		break;
	}
}

//�ÓI�֐�
//-------------------------------------------------------------------------------
	//���ڒ��̊֐����Ă�ł������ł��B�Ƃ肠�����K�v�ȏ�����Z�߂�����

	//���̃N���X���ǂ����ň�x�����ł��g���ꍇ��ԍŏ��ɌĂяo��
void CInput_Handler::Initialize()
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	HWND hwnd = FindWindow(DEF_CLASS_NAME, DEF_WINDOW_CAPTION);
	Keyboard_Initialize(hInst, hwnd);
}
//���̃N���X���ǂ����ň�x�����ł��g������Ăяo��
void CInput_Handler::Finalize()
{
	Keyboard_Finalize();
}

//���̃N���X���ǂ����ň�x�����ł��g������S�̂�update�̍Ō�Ɏg��
void CInput_Handler::Update()
{
	Keyboard_Update();
}