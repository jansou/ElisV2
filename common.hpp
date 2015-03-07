# pragma once
# include <Siv3D.hpp>

enum class Application
{
	Title,
	Elis,
	GreedShooter,
	OekakiMagic,
	Streaks,
	RitoruToransu,
	Hamburger,
	UpUp,
};

	//
	//	�Q�[�����ʃC���^�[�t�F�[�X
	//
	class Game
	{
	public:

		virtual ~Game(){}

		virtual bool init() = 0;

		virtual bool update() = 0;

		virtual Optional<Application> next() = 0;
	};

	//
	//	�|�[�Y��ʍ쐬�N���X
	//
	//		[Esc] �L�[�Ń|�[�Y���j���[���J���悤�ɂȂ�B
	//
	class PauseHelper
	{
	public:

		PauseHelper(
			);

		//
		//	�|�[�Y���Ȃ� true ��Ԃ��B
		//
		//	�|�[�Y���̓J�[�\���ȊO�����`�悷��K�v�͂���܂���B�Q�[���̍X�V���s�v�ł��B
		//
		bool
		onPause(
			) const;

		//
		//	���t���[���ĂԁB
		//
		//	�|�[�Y���Ȃ�|�[�Y���j���[��\�����A�|�[�Y���j���[�̑�����󂯕t����B
		//
		//	menuColor �̓|�[�Y���j���[�̐F
		//
		//		[�Q�[���ɖ߂�] ���I�����ꂽ�ꍇ�A����� pauseEnded() �� true ��Ԃ��B
		//
		//		[���j���[�ɖ߂�] ���I�����ꂽ�ꍇ�Anext �� Application::Title ���i�[�����B
		//
		//		[�I������] ���I�����ꂽ�ꍇ�A������ System::Exit() ���Ă΂��B
		//
		void
		update(
			Optional<Application>& next,
			const Color& menuColor = Palette::Seagreen
			);

		//
		//	���t���[�� PauseHelper::update() �̂��ƂɌĂԁB
		//
		//	�|�[�Y��Ԃɓ���� true ��Ԃ��B
		//
		bool
		pauseStarted(
			);

		//
		//	���t���[�� PauseHelper::update() �̂��ƂɌĂԁB
		//
		//	�|�[�Y����Q�[���ɕ��A����� true ��Ԃ��B
		//
		bool
		returnedToGame(
			);

	private:

		int m_count;

		DynamicTexture m_textureBack;

		bool m_requestCapture;

		bool m_pausing;

		//GUIGroup m_gui;
	};
