#pragma once

#include "Generator.h"
#include "GameManager.h"
#include "AboutForm.h"
#include "ScoreBoardForm.h"
#include "GeneratorForm.h"
#include "HelpForm.h"
#include "ResultDialog.h"
#include "PlayerInformation.h"
#include "SudokuLevel.h"
#include "GameStatus.h"
#include "ScoreBoard.h"
#include "SoundManager.h"
#include "SudokuTool.h"
#include "EWException.h"
#include "Util.h"
#include "ResourceHandler.h"
#include "ConsoleHelper.h"
#include "SplashForm.h"

namespace Sudoku {
	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MapleUI;
	using namespace System::Drawing::Imaging;

	public ref class MainForm sealed : public System::Windows::Forms::Form {
	public:
		static MainForm() {
			initUI();
		}

		MainForm() {
			Thread^ splashThread = gcnew Thread(gcnew ThreadStart(startSplash));
			splashThread->Start();
			Thread::Sleep(5000);
			splashThread->Abort();
			while (splashThread->ThreadState != System::Threading::ThreadState::Aborted) {
			}

			InitializeComponent();
			pbChangePlayStatus->Image = (Image^)resource->getGameResource()->GetObject("PlayStatus");
			cbLevel->SelectedIndex = 0;
			btnHideTabStop->Select();

			lastBackground = -1;
			changeBackground();
			try {
				Cursor = ResourceHandler::loadCustomCursor("./Cursor.cur");
			}
			catch (Exception ^ ex) {
				showMapleMessageBox(ex->Message);
			}
			applyFont();
			updateBGM();
			selectTool = PEN;
		}
	protected:
		static const int BACKGROUND_COUNT = 12;
		static Random^ random = gcnew Random();
		static GameManager^ game = GameManager::getInstance();
		static SoundManager^ sound = SoundManager::getInstance();
		static ResourceHandler^ resource = ResourceHandler::getInstance();

		static AboutForm^ about;
		static ScoreBoardForm^ scoreBoard;
		static GeneratorForm^ generator;
		static HelpForm^ help;

		SudokuTool selectTool;
		int lastBackground;

		~MainForm() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::PictureBox^ pbTable;
		System::Windows::Forms::Label^ labelStep;
		System::Windows::Forms::Label^ labelSolveTime;
		System::Windows::Forms::ToolStripMenuItem^ tsmiGenerator;
		System::Windows::Forms::ToolStripSeparator^ tssGameRank;
		System::Windows::Forms::ToolStripMenuItem^ tsmiScore;
		System::Windows::Forms::ToolStripSeparator^ tssGameOther;
		System::Windows::Forms::ToolStripMenuItem^ tsmiUndo;
		System::Windows::Forms::Timer^ timerCount;
		System::Windows::Forms::PictureBox^ pbInformation;
		System::Windows::Forms::MenuStrip^ msToolBar;
		System::Windows::Forms::ToolStripMenuItem^ tsmiGame;
		System::Windows::Forms::ToolStripMenuItem^ tsmiNewGame;
		System::Windows::Forms::ToolStripMenuItem^ tsmiChangePlayStatus;
		System::Windows::Forms::PictureBox^ pbLevelBackground;
		System::Windows::Forms::Timer^ timerFadeIn;
		System::Windows::Forms::PictureBox^ pbHint;
		System::Windows::Forms::PictureBox^ pbChangePlayStatus;
		System::Windows::Forms::PictureBox^ pbMusic;
		System::Windows::Forms::Label^ labelCurrentLevel;
		System::Windows::Forms::Button^ btnClearNote;
		System::Windows::Forms::ToolStripMenuItem^ tsmiGiveUp;
		System::Windows::Forms::ToolStripMenuItem^ tsmiQuit;
		System::Windows::Forms::ToolStripMenuItem^ tsmiEdit;
		System::Windows::Forms::ToolStripMenuItem^ tsmiCopy;
		System::Windows::Forms::ToolStripMenuItem^ tsmiTool;
		System::Windows::Forms::ToolStripMenuItem^ tsmiHint;
		System::Windows::Forms::ToolStripMenuItem^ tsmiHelp;
		System::Windows::Forms::ToolStripMenuItem^ tsmiAbout;
		System::Windows::Forms::Timer^ timerBackground;
		System::Windows::Forms::Label^ labelLevel;
		System::Windows::Forms::ComboBox^ cbLevel;
		System::Windows::Forms::Button^ btnChangeLevel;
		System::Windows::Forms::PictureBox^ pbPen;
		System::Windows::Forms::PictureBox^ pbPencil;
		System::Windows::Forms::PictureBox^ pbUndo;
		System::Windows::Forms::PictureBox^ pbClearAll;
		System::Windows::Forms::ToolStripMenuItem^ tsmiClearAll;
		System::Windows::Forms::Button^ btnHighScore;
		System::Windows::Forms::Label^ labelGameControl;
		System::Windows::Forms::Label^ labelNote;
		System::Windows::Forms::TextBox^ txtNote;
		System::Windows::Forms::Label^ labelRemainingHint;
		System::Windows::Forms::Label^ labelCurrentTool;
		System::Windows::Forms::ToolStripMenuItem^ tsmiRule;
		System::Windows::Forms::ToolStripSeparator^ tssMusic;
		System::Windows::Forms::ToolStripMenuItem^ tsmiMusic;
		System::Windows::Forms::ToolStripMenuItem^ tsmiConsole;
		System::Windows::Forms::ToolStripMenuItem^ tsmiShowError;
		System::Windows::Forms::ToolStripSeparator^ tssNumberControl;
		System::Windows::Forms::ToolStripSeparator^ tssGenerator;
		System::Windows::Forms::Button^ btnHideTabStop;
		System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent()
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->msToolBar = (gcnew System::Windows::Forms::MenuStrip());
			this->tsmiGame = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiNewGame = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiChangePlayStatus = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiGiveUp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tssMusic = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsmiMusic = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tssGameRank = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsmiScore = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tssGameOther = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsmiQuit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiEdit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tssNumberControl = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsmiUndo = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiClearAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiTool = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiHint = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiShowError = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tssGenerator = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsmiGenerator = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiConsole = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiRule = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pbTable = (gcnew System::Windows::Forms::PictureBox());
			this->labelStep = (gcnew System::Windows::Forms::Label());
			this->labelSolveTime = (gcnew System::Windows::Forms::Label());
			this->timerCount = (gcnew System::Windows::Forms::Timer(this->components));
			this->pbInformation = (gcnew System::Windows::Forms::PictureBox());
			this->timerBackground = (gcnew System::Windows::Forms::Timer(this->components));
			this->labelLevel = (gcnew System::Windows::Forms::Label());
			this->cbLevel = (gcnew System::Windows::Forms::ComboBox());
			this->btnChangeLevel = (gcnew System::Windows::Forms::Button());
			this->btnHighScore = (gcnew System::Windows::Forms::Button());
			this->labelGameControl = (gcnew System::Windows::Forms::Label());
			this->labelNote = (gcnew System::Windows::Forms::Label());
			this->txtNote = (gcnew System::Windows::Forms::TextBox());
			this->labelRemainingHint = (gcnew System::Windows::Forms::Label());
			this->labelCurrentTool = (gcnew System::Windows::Forms::Label());
			this->pbLevelBackground = (gcnew System::Windows::Forms::PictureBox());
			this->timerFadeIn = (gcnew System::Windows::Forms::Timer(this->components));
			this->pbHint = (gcnew System::Windows::Forms::PictureBox());
			this->pbChangePlayStatus = (gcnew System::Windows::Forms::PictureBox());
			this->pbMusic = (gcnew System::Windows::Forms::PictureBox());
			this->labelCurrentLevel = (gcnew System::Windows::Forms::Label());
			this->btnClearNote = (gcnew System::Windows::Forms::Button());
			this->pbPen = (gcnew System::Windows::Forms::PictureBox());
			this->pbPencil = (gcnew System::Windows::Forms::PictureBox());
			this->pbUndo = (gcnew System::Windows::Forms::PictureBox());
			this->pbClearAll = (gcnew System::Windows::Forms::PictureBox());
			this->btnHideTabStop = (gcnew System::Windows::Forms::Button());
			this->msToolBar->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbTable))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbInformation))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbLevelBackground))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbHint))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbChangePlayStatus))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbMusic))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPen))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPencil))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbUndo))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbClearAll))->BeginInit();
			this->SuspendLayout();
			//
			// msToolBar
			//
			this->msToolBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->tsmiGame, this->tsmiEdit,
					this->tsmiTool, this->tsmiConsole, this->tsmiHelp
			});
			this->msToolBar->Location = System::Drawing::Point(0, 0);
			this->msToolBar->Name = L"msToolBar";
			this->msToolBar->Size = System::Drawing::Size(1084, 24);
			this->msToolBar->TabIndex = 0;
			this->msToolBar->Text = L"選單";
			//
			// tsmiGame
			//
			this->tsmiGame->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {
				this->tsmiNewGame,
					this->tsmiChangePlayStatus, this->tsmiGiveUp, this->tssMusic, this->tsmiMusic, this->tssGameRank, this->tsmiScore, this->tssGameOther,
					this->tsmiQuit
			});
			this->tsmiGame->Name = L"tsmiGame";
			this->tsmiGame->Size = System::Drawing::Size(43, 20);
			this->tsmiGame->Text = L"遊戲";
			//
			// tsmiNewGame
			//
			this->tsmiNewGame->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tsmiNewGame.Image")));
			this->tsmiNewGame->Name = L"tsmiNewGame";
			this->tsmiNewGame->Size = System::Drawing::Size(157, 22);
			this->tsmiNewGame->Text = L"開新遊戲";
			this->tsmiNewGame->Click += gcnew System::EventHandler(this, &MainForm::tsmiNewGame_Click);
			//
			// tsmiChangePlayStatus
			//
			this->tsmiChangePlayStatus->Name = L"tsmiChangePlayStatus";
			this->tsmiChangePlayStatus->Size = System::Drawing::Size(157, 22);
			this->tsmiChangePlayStatus->Text = L"暫停 / 繼續遊戲";
			this->tsmiChangePlayStatus->Click += gcnew System::EventHandler(this, &MainForm::tsmiChangePlayStatus_Click);
			//
			// tsmiGiveUp
			//
			this->tsmiGiveUp->Name = L"tsmiGiveUp";
			this->tsmiGiveUp->Size = System::Drawing::Size(157, 22);
			this->tsmiGiveUp->Text = L"放棄遊戲";
			this->tsmiGiveUp->Click += gcnew System::EventHandler(this, &MainForm::tsmiGiveUp_Click);
			//
			// tssMusic
			//
			this->tssMusic->Name = L"tssMusic";
			this->tssMusic->Size = System::Drawing::Size(154, 6);
			//
			// tsmiMusic
			//
			this->tsmiMusic->Name = L"tsmiMusic";
			this->tsmiMusic->Size = System::Drawing::Size(157, 22);
			this->tsmiMusic->Text = L"聲音開關";
			this->tsmiMusic->Click += gcnew System::EventHandler(this, &MainForm::tsmiMusic_Click);
			//
			// tssGameRank
			//
			this->tssGameRank->Name = L"tssGameRank";
			this->tssGameRank->Size = System::Drawing::Size(154, 6);
			//
			// tsmiScore
			//
			this->tsmiScore->Name = L"tsmiScore";
			this->tsmiScore->Size = System::Drawing::Size(157, 22);
			this->tsmiScore->Text = L"高分紀錄";
			this->tsmiScore->Click += gcnew System::EventHandler(this, &MainForm::tsmiScore_Click);
			//
			// tssGameOther
			//
			this->tssGameOther->Name = L"tssGameOther";
			this->tssGameOther->Size = System::Drawing::Size(154, 6);
			//
			// tsmiQuit
			//
			this->tsmiQuit->Name = L"tsmiQuit";
			this->tsmiQuit->Size = System::Drawing::Size(157, 22);
			this->tsmiQuit->Text = L"離開";
			this->tsmiQuit->Click += gcnew System::EventHandler(this, &MainForm::tsmiQuit_Click);
			//
			// tsmiEdit
			//
			this->tsmiEdit->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->tsmiCopy, this->tssNumberControl,
					this->tsmiUndo, this->tsmiClearAll
			});
			this->tsmiEdit->Name = L"tsmiEdit";
			this->tsmiEdit->Size = System::Drawing::Size(43, 20);
			this->tsmiEdit->Text = L"編輯";
			//
			// tsmiCopy
			//
			this->tsmiCopy->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"tsmiCopy.Image")));
			this->tsmiCopy->Name = L"tsmiCopy";
			this->tsmiCopy->Size = System::Drawing::Size(194, 22);
			this->tsmiCopy->Text = L"複製目前狀態到剪貼簿";
			this->tsmiCopy->Click += gcnew System::EventHandler(this, &MainForm::tsmiCopy_Click);
			//
			// tssNumberControl
			//
			this->tssNumberControl->Name = L"tssNumberControl";
			this->tssNumberControl->Size = System::Drawing::Size(191, 6);
			//
			// tsmiUndo
			//
			this->tsmiUndo->Name = L"tsmiUndo";
			this->tsmiUndo->Size = System::Drawing::Size(194, 22);
			this->tsmiUndo->Text = L"復原操作";
			this->tsmiUndo->Click += gcnew System::EventHandler(this, &MainForm::tsmiUndo_Click);
			//
			// tsmiClearAll
			//
			this->tsmiClearAll->Name = L"tsmiClearAll";
			this->tsmiClearAll->Size = System::Drawing::Size(194, 22);
			this->tsmiClearAll->Text = L"清除所有數字";
			this->tsmiClearAll->Click += gcnew System::EventHandler(this, &MainForm::tsmiClearAll_Click);
			//
			// tsmiTool
			//
			this->tsmiTool->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->tsmiHint, this->tsmiShowError,
					this->tssGenerator, this->tsmiGenerator
			});
			this->tsmiTool->Name = L"tsmiTool";
			this->tsmiTool->Size = System::Drawing::Size(43, 20);
			this->tsmiTool->Text = L"工具";
			//
			// tsmiHint
			//
			this->tsmiHint->Name = L"tsmiHint";
			this->tsmiHint->Size = System::Drawing::Size(146, 22);
			this->tsmiHint->Text = L"提示";
			this->tsmiHint->Click += gcnew System::EventHandler(this, &MainForm::tsmiHint_Click);
			//
			// tsmiShowError
			//
			this->tsmiShowError->Checked = true;
			this->tsmiShowError->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tsmiShowError->Name = L"tsmiShowError";
			this->tsmiShowError->Size = System::Drawing::Size(146, 22);
			this->tsmiShowError->Text = L"顯示錯誤";
			this->tsmiShowError->Click += gcnew System::EventHandler(this, &MainForm::tsmiShowError_Click);
			//
			// tssGenerator
			//
			this->tssGenerator->Name = L"tssGenerator";
			this->tssGenerator->Size = System::Drawing::Size(143, 6);
			//
			// tsmiGenerator
			//
			this->tsmiGenerator->Name = L"tsmiGenerator";
			this->tsmiGenerator->Size = System::Drawing::Size(146, 22);
			this->tsmiGenerator->Text = L"題目產生工具";
			this->tsmiGenerator->Click += gcnew System::EventHandler(this, &MainForm::tsmiGenerator_Click);
			//
			// tsmiConsole
			//
			this->tsmiConsole->Name = L"tsmiConsole";
			this->tsmiConsole->Size = System::Drawing::Size(55, 20);
			this->tsmiConsole->Text = L"主控台";
			this->tsmiConsole->Click += gcnew System::EventHandler(this, &MainForm::tsmiConsole_Click);
			//
			// tsmiHelp
			//
			this->tsmiHelp->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiRule, this->tsmiAbout });
			this->tsmiHelp->Name = L"tsmiHelp";
			this->tsmiHelp->Size = System::Drawing::Size(43, 20);
			this->tsmiHelp->Text = L"說明";
			//
			// tsmiRule
			//
			this->tsmiRule->Name = L"tsmiRule";
			this->tsmiRule->Size = System::Drawing::Size(98, 22);
			this->tsmiRule->Text = L"規則";
			this->tsmiRule->Click += gcnew System::EventHandler(this, &MainForm::tsmiRule_Click);
			//
			// tsmiAbout
			//
			this->tsmiAbout->Name = L"tsmiAbout";
			this->tsmiAbout->Size = System::Drawing::Size(98, 22);
			this->tsmiAbout->Text = L"關於";
			this->tsmiAbout->Click += gcnew System::EventHandler(this, &MainForm::tsmiAbout_Click);
			//
			// pbTable
			//
			this->pbTable->BackColor = System::Drawing::Color::Transparent;
			this->pbTable->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbTable.Image")));
			this->pbTable->Location = System::Drawing::Point(1084, 24);
			this->pbTable->Name = L"pbTable";
			this->pbTable->Size = System::Drawing::Size(700, 637);
			this->pbTable->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pbTable->TabIndex = 1;
			this->pbTable->TabStop = false;
			this->pbTable->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pbTable_MouseDown);
			//
			// labelStep
			//
			this->labelStep->AutoSize = true;
			this->labelStep->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->labelStep->ForeColor = System::Drawing::Color::White;
			this->labelStep->Location = System::Drawing::Point(60, 310);
			this->labelStep->Name = L"labelStep";
			this->labelStep->Size = System::Drawing::Size(68, 12);
			this->labelStep->TabIndex = 8;
			this->labelStep->Text = L"解題步數 : 0";
			//
			// labelSolveTime
			//
			this->labelSolveTime->AutoSize = true;
			this->labelSolveTime->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->labelSolveTime->ForeColor = System::Drawing::Color::White;
			this->labelSolveTime->Location = System::Drawing::Point(60, 335);
			this->labelSolveTime->Name = L"labelSolveTime";
			this->labelSolveTime->Size = System::Drawing::Size(83, 12);
			this->labelSolveTime->TabIndex = 9;
			this->labelSolveTime->Text = L"解題時間 : 0 秒";
			//
			// timerCount
			//
			this->timerCount->Enabled = true;
			this->timerCount->Interval = 1000;
			this->timerCount->Tick += gcnew System::EventHandler(this, &MainForm::timerCount_Tick);
			//
			// pbInformation
			//
			this->pbInformation->BackColor = System::Drawing::Color::Transparent;
			this->pbInformation->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbInformation.Image")));
			this->pbInformation->Location = System::Drawing::Point(40, 265);
			this->pbInformation->Name = L"pbInformation";
			this->pbInformation->Size = System::Drawing::Size(290, 100);
			this->pbInformation->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbInformation->TabIndex = 4;
			this->pbInformation->TabStop = false;
			//
			// timerBackground
			//
			this->timerBackground->Enabled = true;
			this->timerBackground->Interval = 60000;
			this->timerBackground->Tick += gcnew System::EventHandler(this, &MainForm::timerBackground_Tick);
			//
			// labelLevel
			//
			this->labelLevel->AutoSize = true;
			this->labelLevel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)), static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->labelLevel->Location = System::Drawing::Point(50, 53);
			this->labelLevel->Name = L"labelLevel";
			this->labelLevel->Size = System::Drawing::Size(53, 12);
			this->labelLevel->TabIndex = 1;
			this->labelLevel->Text = L"難度設定";
			//
			// cbLevel
			//
			this->cbLevel->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbLevel->FormattingEnabled = true;
			this->cbLevel->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"簡單", L"中等", L"困難", L"地獄" });
			this->cbLevel->Location = System::Drawing::Point(109, 50);
			this->cbLevel->Name = L"cbLevel";
			this->cbLevel->Size = System::Drawing::Size(121, 20);
			this->cbLevel->TabIndex = 2;
			//
			// btnChangeLevel
			//
			this->btnChangeLevel->Location = System::Drawing::Point(236, 48);
			this->btnChangeLevel->Name = L"btnChangeLevel";
			this->btnChangeLevel->Size = System::Drawing::Size(75, 23);
			this->btnChangeLevel->TabIndex = 3;
			this->btnChangeLevel->Text = L"選擇難度";
			this->btnChangeLevel->UseVisualStyleBackColor = true;
			this->btnChangeLevel->Click += gcnew System::EventHandler(this, &MainForm::btnChangeLevel_Click);
			//
			// btnHighScore
			//
			this->btnHighScore->Location = System::Drawing::Point(12, 626);
			this->btnHighScore->Name = L"btnHighScore";
			this->btnHighScore->Size = System::Drawing::Size(75, 23);
			this->btnHighScore->TabIndex = 13;
			this->btnHighScore->Text = L"高分紀錄";
			this->btnHighScore->UseVisualStyleBackColor = true;
			this->btnHighScore->Click += gcnew System::EventHandler(this, &MainForm::btnHighScore_Click);
			//
			// labelGameControl
			//
			this->labelGameControl->AutoSize = true;
			this->labelGameControl->BackColor = System::Drawing::Color::Transparent;
			this->labelGameControl->Location = System::Drawing::Point(12, 95);
			this->labelGameControl->Name = L"labelGameControl";
			this->labelGameControl->Size = System::Drawing::Size(53, 12);
			this->labelGameControl->TabIndex = 4;
			this->labelGameControl->Text = L"遊戲控制";
			//
			// labelNote
			//
			this->labelNote->AutoSize = true;
			this->labelNote->BackColor = System::Drawing::Color::Transparent;
			this->labelNote->Location = System::Drawing::Point(12, 465);
			this->labelNote->Name = L"labelNote";
			this->labelNote->Size = System::Drawing::Size(53, 12);
			this->labelNote->TabIndex = 10;
			this->labelNote->Text = L"隨意筆記";
			//
			// txtNote
			//
			this->txtNote->BackColor = System::Drawing::Color::Azure;
			this->txtNote->Location = System::Drawing::Point(25, 505);
			this->txtNote->Multiline = true;
			this->txtNote->Name = L"txtNote";
			this->txtNote->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtNote->Size = System::Drawing::Size(330, 100);
			this->txtNote->TabIndex = 12;
			//
			// labelRemainingHint
			//
			this->labelRemainingHint->AutoSize = true;
			this->labelRemainingHint->BackColor = System::Drawing::Color::Transparent;
			this->labelRemainingHint->Location = System::Drawing::Point(40, 230);
			this->labelRemainingHint->Name = L"labelRemainingHint";
			this->labelRemainingHint->Size = System::Drawing::Size(44, 12);
			this->labelRemainingHint->TabIndex = 6;
			this->labelRemainingHint->Text = L"剩餘 : 0";
			//
			// labelCurrentTool
			//
			this->labelCurrentTool->AutoSize = true;
			this->labelCurrentTool->BackColor = System::Drawing::Color::Transparent;
			this->labelCurrentTool->Location = System::Drawing::Point(235, 95);
			this->labelCurrentTool->Name = L"labelCurrentTool";
			this->labelCurrentTool->Size = System::Drawing::Size(98, 12);
			this->labelCurrentTool->TabIndex = 5;
			this->labelCurrentTool->Text = L"填寫工具 : 原子筆";
			//
			// pbLevelBackground
			//
			this->pbLevelBackground->BackColor = System::Drawing::Color::Transparent;
			this->pbLevelBackground->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbLevelBackground.Image")));
			this->pbLevelBackground->Location = System::Drawing::Point(15, 40);
			this->pbLevelBackground->Name = L"pbLevelBackground";
			this->pbLevelBackground->Size = System::Drawing::Size(340, 40);
			this->pbLevelBackground->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbLevelBackground->TabIndex = 21;
			this->pbLevelBackground->TabStop = false;
			//
			// timerFadeIn
			//
			this->timerFadeIn->Tick += gcnew System::EventHandler(this, &MainForm::timerFadeIn_Tick);
			//
			// pbHint
			//
			this->pbHint->BackColor = System::Drawing::Color::Transparent;
			this->pbHint->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbHint.Image")));
			this->pbHint->Location = System::Drawing::Point(30, 135);
			this->pbHint->Name = L"pbHint";
			this->pbHint->Size = System::Drawing::Size(90, 90);
			this->pbHint->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbHint->TabIndex = 22;
			this->pbHint->TabStop = false;
			this->pbHint->Click += gcnew System::EventHandler(this, &MainForm::pbHint_Click);
			//
			// pbChangePlayStatus
			//
			this->pbChangePlayStatus->BackColor = System::Drawing::Color::Transparent;
			this->pbChangePlayStatus->Location = System::Drawing::Point(135, 135);
			this->pbChangePlayStatus->Name = L"pbChangePlayStatus";
			this->pbChangePlayStatus->Size = System::Drawing::Size(90, 90);
			this->pbChangePlayStatus->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbChangePlayStatus->TabIndex = 23;
			this->pbChangePlayStatus->TabStop = false;
			this->pbChangePlayStatus->Click += gcnew System::EventHandler(this, &MainForm::pbChangePlayStatus_Click);
			//
			// pbMusic
			//
			this->pbMusic->BackColor = System::Drawing::Color::Transparent;
			this->pbMusic->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbMusic.Image")));
			this->pbMusic->Location = System::Drawing::Point(240, 135);
			this->pbMusic->Name = L"pbMusic";
			this->pbMusic->Size = System::Drawing::Size(90, 90);
			this->pbMusic->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbMusic->TabIndex = 24;
			this->pbMusic->TabStop = false;
			this->pbMusic->Click += gcnew System::EventHandler(this, &MainForm::pbMusic_Click);
			//
			// labelCurrentLevel
			//
			this->labelCurrentLevel->AutoSize = true;
			this->labelCurrentLevel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->labelCurrentLevel->ForeColor = System::Drawing::Color::White;
			this->labelCurrentLevel->Location = System::Drawing::Point(60, 285);
			this->labelCurrentLevel->Name = L"labelCurrentLevel";
			this->labelCurrentLevel->Size = System::Drawing::Size(110, 12);
			this->labelCurrentLevel->TabIndex = 7;
			this->labelCurrentLevel->Text = L"解題難度 : 尚未遊玩";
			//
			// btnClearNote
			//
			this->btnClearNote->Location = System::Drawing::Point(280, 460);
			this->btnClearNote->Name = L"btnClearNote";
			this->btnClearNote->Size = System::Drawing::Size(75, 23);
			this->btnClearNote->TabIndex = 11;
			this->btnClearNote->Text = L"清除筆記";
			this->btnClearNote->UseVisualStyleBackColor = true;
			this->btnClearNote->Click += gcnew System::EventHandler(this, &MainForm::btnClearNote_Click);
			//
			// pbPen
			//
			this->pbPen->BackColor = System::Drawing::Color::Transparent;
			this->pbPen->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbPen.Image")));
			this->pbPen->Location = System::Drawing::Point(70, 385);
			this->pbPen->Name = L"pbPen";
			this->pbPen->Size = System::Drawing::Size(40, 40);
			this->pbPen->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbPen->TabIndex = 27;
			this->pbPen->TabStop = false;
			this->pbPen->Click += gcnew System::EventHandler(this, &MainForm::pbPen_Click);
			//
			// pbPencil
			//
			this->pbPencil->BackColor = System::Drawing::Color::Transparent;
			this->pbPencil->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbPencil.Image")));
			this->pbPencil->Location = System::Drawing::Point(120, 385);
			this->pbPencil->Name = L"pbPencil";
			this->pbPencil->Size = System::Drawing::Size(40, 40);
			this->pbPencil->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbPencil->TabIndex = 28;
			this->pbPencil->TabStop = false;
			this->pbPencil->Click += gcnew System::EventHandler(this, &MainForm::pbPencil_Click);
			//
			// pbUndo
			//
			this->pbUndo->BackColor = System::Drawing::Color::Transparent;
			this->pbUndo->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbUndo.Image")));
			this->pbUndo->Location = System::Drawing::Point(200, 385);
			this->pbUndo->Name = L"pbUndo";
			this->pbUndo->Size = System::Drawing::Size(40, 40);
			this->pbUndo->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbUndo->TabIndex = 29;
			this->pbUndo->TabStop = false;
			this->pbUndo->Click += gcnew System::EventHandler(this, &MainForm::pbUndo_Click);
			//
			// pbClearAll
			//
			this->pbClearAll->BackColor = System::Drawing::Color::Transparent;
			this->pbClearAll->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbClearAll.Image")));
			this->pbClearAll->Location = System::Drawing::Point(250, 385);
			this->pbClearAll->Name = L"pbClearAll";
			this->pbClearAll->Size = System::Drawing::Size(40, 40);
			this->pbClearAll->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbClearAll->TabIndex = 30;
			this->pbClearAll->TabStop = false;
			this->pbClearAll->Click += gcnew System::EventHandler(this, &MainForm::pbClearAll_Click);
			//
			// btnHideTabStop
			//
			this->btnHideTabStop->Location = System::Drawing::Point(-1, -1);
			this->btnHideTabStop->Name = L"btnHideTabStop";
			this->btnHideTabStop->Size = System::Drawing::Size(0, 0);
			this->btnHideTabStop->TabIndex = 14;
			this->btnHideTabStop->Visible = false;
			//
			// MainForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(1084, 661);
			this->Controls->Add(this->btnHideTabStop);
			this->Controls->Add(this->pbClearAll);
			this->Controls->Add(this->pbUndo);
			this->Controls->Add(this->pbPencil);
			this->Controls->Add(this->pbPen);
			this->Controls->Add(this->btnClearNote);
			this->Controls->Add(this->labelCurrentLevel);
			this->Controls->Add(this->pbMusic);
			this->Controls->Add(this->pbChangePlayStatus);
			this->Controls->Add(this->pbHint);
			this->Controls->Add(this->labelCurrentTool);
			this->Controls->Add(this->labelRemainingHint);
			this->Controls->Add(this->txtNote);
			this->Controls->Add(this->labelNote);
			this->Controls->Add(this->labelGameControl);
			this->Controls->Add(this->btnHighScore);
			this->Controls->Add(this->btnChangeLevel);
			this->Controls->Add(this->cbLevel);
			this->Controls->Add(this->labelLevel);
			this->Controls->Add(this->labelSolveTime);
			this->Controls->Add(this->labelStep);
			this->Controls->Add(this->pbInformation);
			this->Controls->Add(this->pbTable);
			this->Controls->Add(this->msToolBar);
			this->Controls->Add(this->pbLevelBackground);
			this->DoubleBuffered = true;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->KeyPreview = true;
			this->MainMenuStrip = this->msToolBar;
			this->MinimumSize = System::Drawing::Size(1100, 700);
			this->Name = L"MainForm";
			this->Text = L"數獨";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->SizeChanged += gcnew System::EventHandler(this, &MainForm::MainForm_SizeChanged);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::MainForm_KeyDown);
			this->msToolBar->ResumeLayout(false);
			this->msToolBar->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbTable))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbInformation))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbLevelBackground))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbHint))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbChangePlayStatus))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbMusic))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPen))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPencil))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbUndo))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbClearAll))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		static void startSplash() {
			Application::Run(gcnew SplashForm());
		}

		void forceBringToFront() {
			WindowState = FormWindowState::Minimized;
			WindowState = FormWindowState::Normal;
		}

		System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
			if (GameManager::hasSave()) {
				if (MessageBox::Show("找到遊戲存檔 請問是否讀取之前的紀錄 ?", "確認", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
					game->loadGame();
					updateStatus();
				}
				try {
					File::Delete(GameManager::getRecordFileName());
				}
				catch (Exception ^ ex) {
					Console::WriteLine("刪除遊戲紀錄檔案時發生例外狀況 : " + ex->Message);
				}
			}
			forceBringToFront();
			timerFadeIn->Start();
		}

		static AboutForm^ getAboutForm() {
			if (about == nullptr || about->IsDisposed) {
				about = gcnew AboutForm();
			}
			return about;
		}

		static void updateBGM() {
			if (game->getStatus() == GAME_OVER) {
				sound->playBGM("IdleBGM.mp3");
			}
			else {
				sound->playBGM("PlayBGM.mp3");
			}
		}

		void undo() {
			if (game->getStatus() != PLAYING) {
				return;
			}
			if (!game->getTable()->undo()) {
				showMapleMessageBox("沒有任何歷史操作");
				return;
			}
			checkTable();
			updateStatus();
		}

		void hint() {
			if (game->getStatus() != PLAYING) {
				return;
			}
			if (game->getTable()->getRemainingHint() <= 0) {
				showMapleMessageBox("提示次數已用完" + Environment::NewLine + "本局遊戲無法再使用");
				return;
			}
			if (!game->getTable()->isSelect()) {
				showMapleMessageBox("請先指定位置");
				return;
			}
			game->getTable()->requestHint();
			checkTable();

			game->getTable()->setNoSelect();
			updateStatus();
		}

		static void showMapleMessageBox(String^ message) {
			(gcnew MapleMessageBox(message))->Show();
		}

		void togglePause() {
			if (game->getStatus() == GAME_OVER) {
				return;
			}
			game->togglePause();
			updateStatus();
		}

		void clearAll() {
			if (game->getStatus() != PLAYING) {
				return;
			}
			if (MessageBox::Show("確定清除所有數字 ?", "確認", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
				game->getTable()->clearFillNumber();
				updateStatus();
			}
		}

		static void showScoreBoard() {
			scoreBoard->loadScoreBoard();
			scoreBoard->Show();
		}

		void changeBackground() {
			int select = random->Next(BACKGROUND_COUNT) + 1;
			while (select == lastBackground) {
				select = random->Next(BACKGROUND_COUNT) + 1;
			}
			lastBackground = select;
			BackgroundImage = (Image^)resource->getGameResource()->GetObject("Background_" + select);
		}

		static void initUI() {
			generator = gcnew GeneratorForm();
			generator->applyFont();

			scoreBoard = gcnew ScoreBoardForm();
			scoreBoard->applyFont();

			help = gcnew HelpForm();
			help->applyFont();
		}

		static void playGameOverSound() {
			sound->getSoundPlayer()->Stream = resource->getGameResource()->GetStream("GameOver");
			sound->getSoundPlayer()->Play();
		}

		System::Void timerBackground_Tick(System::Object^ sender, System::EventArgs^ e) {
			changeBackground();
		}

		System::Void tsmiAbout_Click(System::Object^ sender, System::EventArgs^ e) {
			getAboutForm()->Show();
		}

		System::Void tsmiGenerator_Click(System::Object^ sender, System::EventArgs^ e) {
			generator->Show();
		}

		System::Void timerCount_Tick(System::Object^ sender, System::EventArgs^ e) {
			if (game->getStatus() == PLAYING) {
				game->gameTimerTick();
			}
			updateStatus();
		}

		void updateTable() {
			if (game->getTable() == nullptr) {
				return;
			}
			if (game->getStatus() == PAUSE) {
				pbTable->Image = (Image^)resource->getGameResource()->GetObject("PauseTable");
				return;
			}

			Graphics^ graphic = nullptr;
			try {
				Bitmap^ bmp = gcnew Bitmap(pbTable->Width, pbTable->Height, PixelFormat::Format32bppArgb);
				bmp->MakeTransparent();

				graphic = Graphics::FromImage(bmp);
				game->getTable()->drawTable(graphic);
				pbTable->Image = bmp;
			}
			catch (...) {
			}
			finally {
				if (graphic != nullptr) {
					graphic->~Graphics();
				}
			}
		}

		void updateStatus() {
			pbChangePlayStatus->Image = game->getStatus() == PLAYING ? (Image^)resource->getGameResource()->GetObject("PauseStatus") : (Image^)resource->getGameResource()->GetObject("PlayStatus");
			if (game->getStatus() == GAME_OVER) {
				labelRemainingHint->Text = "剩餘 : 0";
				labelCurrentLevel->Text = "解題難度 : 尚未遊玩";
				labelStep->Text = "解題步數 : 0";
				labelSolveTime->Text = "解題時間 : 0 秒";
			}
			else {
				labelRemainingHint->Text = "剩餘 : " + game->getTable()->getRemainingHint();
				labelCurrentLevel->Text = "解題難度 : " + getLevelName(game->getLevel());
				labelStep->Text = "解題步數 : " + game->getSolveStep();
				labelSolveTime->Text = "解題時間 : " + makeTimeReadable(game->getPlayingTime());
			}
			labelCurrentTool->Text = "填寫工具 : " + getSudokuToolName(selectTool);
			tsmiShowError->Checked = showError;

			updateTable();
			updateBGM();
		}

		System::Void tsmiRule_Click(System::Object^ sender, System::EventArgs^ e) {
			help->Show();
		}

		System::Void tsmiQuit_Click(System::Object^ sender, System::EventArgs^ e) {
			if (MessageBox::Show("確定離開遊戲 ?", "確認", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
				Close();
			}
		}

		System::Void tsmiScore_Click(System::Object^ sender, System::EventArgs^ e) {
			showScoreBoard();
		}

		System::Void tsmiGiveUp_Click(System::Object^ sender, System::EventArgs^ e) {
			if (game->getStatus() == GAME_OVER) {
				return;
			}
			if (MessageBox::Show("確定放棄遊戲 ?", "確認", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
				game->getTable()->setNoSelect();
				game->giveUp();
				updateStatus();

				showMapleMessageBox("已放棄本次遊戲");
				playGameOverSound();
			}
		}

		System::Void tsmiNewGame_Click(System::Object^ sender, System::EventArgs^ e) {
			if (game->getStatus() == GAME_OVER || MessageBox::Show("遊戲仍在進行中 請問是否建立新局 ?", "確認", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
				game->newGame();
				updateStatus();
			}
		}

		System::Void tsmiCopy_Click(System::Object^ sender, System::EventArgs^ e) {
			if (game->getStatus() != PLAYING) {
				return;
			}
			Clipboard::SetText(game->getTable()->getGameTableString());
			showMapleMessageBox("已複製目前狀態到剪貼簿");
		}

		System::Void tsmiHint_Click(System::Object^ sender, System::EventArgs^ e) {
			hint();
		}

		void applyFont() {
			System::Drawing::Font^ displayLargeTitle = resource->getDisplayLargeTitleFont();
			System::Drawing::Font^ displayTitle = resource->getDisplayTitleFont();
			System::Drawing::Font^ displayContent = resource->getDisplayContentFont();
			labelLevel->Font = displayContent;
			cbLevel->Font = displayContent;
			btnChangeLevel->Font = displayContent;

			labelGameControl->Font = displayLargeTitle;
			labelCurrentTool->Font = displayTitle;
			labelRemainingHint->Font = displayTitle;

			labelCurrentLevel->Font = displayTitle;
			labelStep->Font = displayTitle;
			labelSolveTime->Font = displayTitle;

			labelNote->Font = displayLargeTitle;
			txtNote->Font = displayContent;
			btnClearNote->Font = displayContent;
			btnHighScore->Font = displayContent;
		}

		System::Void pbTable_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
			btnHideTabStop->Select();
			if (game->getStatus() != PLAYING) {
				return;
			}
			if (e->Button == System::Windows::Forms::MouseButtons::Left) {
				game->getTable()->setCurrentLocation(e->Location);
				updateStatus();
			}
		}

		void setNoSelect() {
			game->getTable()->setNoSelect();
			updateStatus();
		}

		static void checkTable() { // 僅檢查無更新狀態
			int emptyCount = game->getTable()->checkTable();
			if (emptyCount <= 0 && !game->getTable()->hasError()) {
				game->complete();
				int finalScore = game->getScore();
				int solveTime = game->getPlayingTime();
				SudokuLevel level = game->getLevel();
				playGameOverSound();
				triggerResultDialog(finalScore, solveTime, level);
			}
		}

		System::Void MainForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (game->getStatus() != PLAYING) {
				return;
			}

			int value = e->KeyValue;
			if (value == (int)Keys::Escape) {
				setNoSelect();
				return;
			}
			if (e->Control && value == (int)Keys::Z) {
				undo();
				return;
			}

			if (!game->getTable()->isSelect()) {
				return;
			}
			if (value == (int)Keys::Delete) {
				if (selectTool == PEN) {
					game->getTable()->set(0);
				}
				else {
					game->getTable()->pencil(0);
				}
				checkTable();
				setNoSelect();
				return;
			}

			if (value >= (int)Keys::NumPad0 && value <= (int)Keys::NumPad9) {
				value -= 48;
			}
			if (value < '0' || value > '9') {
				return;
			}
			if (selectTool == PEN) {
				bool hasSet = game->getTable()->set(value - '0');
				if (hasSet && value != '0') {
					game->addSolveStep();
					if (showError) {
						if (game->getTable()->checkCurrentLocation()) {
							sound->getSoundPlayer()->Stream = resource->getGameResource()->GetStream("Right");
						}
						else {
							sound->getSoundPlayer()->Stream = resource->getGameResource()->GetStream("Wrong");
						}
						sound->getSoundPlayer()->Play();
					}
				}
				checkTable();
			}
			else {
				game->getTable()->pencil(value - '0');
			}
			setNoSelect();
		}

		static void triggerResultDialog(int score, int solveTime, SudokuLevel level) {
			ResultDialog^ dialog = gcnew ResultDialog(score, solveTime, level);
			dialog->applyFont();
			dialog->Show();
		}

		System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			Generator::stopAll();
			if (game->getStatus() != GAME_OVER) {
				game->saveGame();
			}
		}

		System::Void tsmiUndo_Click(System::Object^ sender, System::EventArgs^ e) {
			undo();
		}

		System::Void btnChangeLevel_Click(System::Object^ sender, System::EventArgs^ e) {
			if (game->getStatus() == GAME_OVER || MessageBox::Show("遊戲仍在進行中 變更難度將導致遊戲重置 請問是否繼續 ?", "確認", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
				SudokuLevel level = (SudokuLevel)cbLevel->SelectedIndex;
				game->changeLevel(level);
				updateStatus();
			}
		}

		System::Void btnHighScore_Click(System::Object^ sender, System::EventArgs^ e) {
			showScoreBoard();
		}

		System::Void tsmiClearAll_Click(System::Object^ sender, System::EventArgs^ e) {
			clearAll();
		}

		System::Void timerFadeIn_Tick(System::Object^ sender, System::EventArgs^ e) {
			pbTable->Left -= 50;
			Application::DoEvents(); // 盡可能避免破圖
			if (pbTable->Left <= 384) {
				timerFadeIn->Stop();
			}
		}

		System::Void pbHint_Click(System::Object^ sender, System::EventArgs^ e) {
			hint();
		}

		System::Void pbChangePlayStatus_Click(System::Object^ sender, System::EventArgs^ e) {
			togglePause();
		}

		System::Void tsmiChangePlayStatus_Click(System::Object^ sender, System::EventArgs^ e) {
			togglePause();
		}

		System::Void pbMusic_Click(System::Object^ sender, System::EventArgs^ e) {
			sound->toggleMuteBGM();
		}

		System::Void btnClearNote_Click(System::Object^ sender, System::EventArgs^ e) {
			txtNote->Text = "";
		}

		System::Void pbPen_Click(System::Object^ sender, System::EventArgs^ e) {
			selectTool = PEN;
			updateStatus();
		}

		System::Void pbPencil_Click(System::Object^ sender, System::EventArgs^ e) {
			selectTool = PENCIL;
			updateStatus();
		}

		System::Void pbUndo_Click(System::Object^ sender, System::EventArgs^ e) {
			undo();
		}

		System::Void pbClearAll_Click(System::Object^ sender, System::EventArgs^ e) {
			clearAll();
		}

		System::Void MainForm_SizeChanged(System::Object^ sender, System::EventArgs^ e) {
			pbTable->Size = System::Drawing::Size(700 + (Width - 1100), 637 + (Height - 700));
			updateStatus();
		}

		System::Void tsmiMusic_Click(System::Object^ sender, System::EventArgs^ e) {
			sound->toggleMuteBGM();
		}

		System::Void tsmiConsole_Click(System::Object^ sender, System::EventArgs^ e) {
			ConsoleHelper::getInstance("數獨")->toggleConsole();
		}

		System::Void tsmiShowError_Click(System::Object^ sender, System::EventArgs^ e) {
			toggleShowError();
			updateStatus();
		}
	};
}