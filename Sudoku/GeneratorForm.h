#pragma once

#include "Generator.h"
#include "LogEvent.h"
#include "ResourceHandler.h"

namespace Sudoku {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace SudokuGenerator;
	using namespace MapleUI;

	public ref class GeneratorForm sealed : public System::Windows::Forms::Form {
	public:
		GeneratorForm() {
			InitializeComponent();
			registerEvent();

			threadLimit = Environment::ProcessorCount / 2 - 1;
			if (threadLimit <= 0) {
				threadLimit = 1;
			}
			txtThread->Text = threadLimit.ToString();
		}
	protected:
		int threadLimit;

		~GeneratorForm() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::Label^ labelOutput;
		System::Windows::Forms::TextBox^ txtOutput;
		System::Windows::Forms::Label^ labelThread;
		System::Windows::Forms::TextBox^ txtThread;
		System::Windows::Forms::Label^ labelPuzzle;
		System::Windows::Forms::TextBox^ txtPuzzle;
		System::Windows::Forms::Button^ btnStart;
		System::Windows::Forms::Button^ btnStop;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent()
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(GeneratorForm::typeid));
			this->labelOutput = (gcnew System::Windows::Forms::Label());
			this->txtOutput = (gcnew System::Windows::Forms::TextBox());
			this->labelThread = (gcnew System::Windows::Forms::Label());
			this->txtThread = (gcnew System::Windows::Forms::TextBox());
			this->labelPuzzle = (gcnew System::Windows::Forms::Label());
			this->txtPuzzle = (gcnew System::Windows::Forms::TextBox());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->btnStop = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			//
			// labelOutput
			//
			this->labelOutput->AutoSize = true;
			this->labelOutput->Location = System::Drawing::Point(12, 9);
			this->labelOutput->Name = L"labelOutput";
			this->labelOutput->Size = System::Drawing::Size(53, 12);
			this->labelOutput->TabIndex = 0;
			this->labelOutput->Text = L"輸出紀錄";
			//
			// txtOutput
			//
			this->txtOutput->Location = System::Drawing::Point(25, 40);
			this->txtOutput->Multiline = true;
			this->txtOutput->Name = L"txtOutput";
			this->txtOutput->ReadOnly = true;
			this->txtOutput->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtOutput->Size = System::Drawing::Size(345, 305);
			this->txtOutput->TabIndex = 1;
			//
			// labelThread
			//
			this->labelThread->AutoSize = true;
			this->labelThread->Location = System::Drawing::Point(12, 365);
			this->labelThread->Name = L"labelThread";
			this->labelThread->Size = System::Drawing::Size(65, 12);
			this->labelThread->TabIndex = 2;
			this->labelThread->Text = L"執行緒數量";
			//
			// txtThread
			//
			this->txtThread->Location = System::Drawing::Point(83, 362);
			this->txtThread->Name = L"txtThread";
			this->txtThread->Size = System::Drawing::Size(100, 22);
			this->txtThread->TabIndex = 3;
			this->txtThread->Text = L"2";
			this->txtThread->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			//
			// labelPuzzle
			//
			this->labelPuzzle->AutoSize = true;
			this->labelPuzzle->Location = System::Drawing::Point(215, 365);
			this->labelPuzzle->Name = L"labelPuzzle";
			this->labelPuzzle->Size = System::Drawing::Size(65, 12);
			this->labelPuzzle->TabIndex = 4;
			this->labelPuzzle->Text = L"產出題目數";
			//
			// txtPuzzle
			//
			this->txtPuzzle->Location = System::Drawing::Point(286, 362);
			this->txtPuzzle->Name = L"txtPuzzle";
			this->txtPuzzle->Size = System::Drawing::Size(100, 22);
			this->txtPuzzle->TabIndex = 5;
			this->txtPuzzle->Text = L"2";
			this->txtPuzzle->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			//
			// btnStart
			//
			this->btnStart->Location = System::Drawing::Point(100, 400);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(75, 23);
			this->btnStart->TabIndex = 6;
			this->btnStart->Text = L"開始";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &GeneratorForm::btnStart_Click);
			//
			// btnStop
			//
			this->btnStop->Enabled = false;
			this->btnStop->Location = System::Drawing::Point(205, 400);
			this->btnStop->Name = L"btnStop";
			this->btnStop->Size = System::Drawing::Size(75, 23);
			this->btnStop->TabIndex = 7;
			this->btnStop->Text = L"停止";
			this->btnStop->UseVisualStyleBackColor = true;
			this->btnStop->Click += gcnew System::EventHandler(this, &GeneratorForm::btnStop_Click);
			//
			// GeneratorForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(399, 441);
			this->Controls->Add(this->btnStop);
			this->Controls->Add(this->btnStart);
			this->Controls->Add(this->txtPuzzle);
			this->Controls->Add(this->labelPuzzle);
			this->Controls->Add(this->txtThread);
			this->Controls->Add(this->labelThread);
			this->Controls->Add(this->txtOutput);
			this->Controls->Add(this->labelOutput);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->Name = L"GeneratorForm";
			this->Text = L"數獨題目產生器";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &GeneratorForm::GeneratorForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		System::Void btnStart_Click(System::Object^ sender, System::EventArgs^ e) {
			try {
				int thread = Int32::Parse(txtThread->Text);
				int puzzle = Int32::Parse(txtPuzzle->Text);
				if (thread <= 0 || puzzle <= 0) {
					(gcnew MapleMessageBox("輸入數值錯誤"))->Show();
					return;
				}
				if (thread > threadLimit) {
					(gcnew MapleMessageBox("為保證遊戲流暢度" + Environment::NewLine + "執行緒數量請不要超出 " + threadLimit + " 個"))->Show();
					return;
				}
				Generator::generate(thread, puzzle);

				Thread^ watchDogThread = gcnew Thread(gcnew ThreadStart(&Generator::watchDog));
				watchDogThread->Start();
				enableControl(true);
			}
			catch (Exception ^ ex) {
				(gcnew MapleMessageBox("起始作業時發生例外狀況" + Environment::NewLine + ex->Message))->Show();
			}
		}

		System::Void btnStop_Click(System::Object^ sender, System::EventArgs^ e) {
			Generator::stopAll();
		}

		System::Void GeneratorForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			Hide();
			e->Cancel = true;
		}

		delegate void AppendOutputDelegate(String^ message);
		delegate void ControlEnableDelegate(bool start);

		void appendOutput(String^ message) {
			try {
				if (txtOutput->InvokeRequired) {
					AppendOutputDelegate^ delegateFunction = gcnew AppendOutputDelegate(this, &GeneratorForm::appendOutput);
					Invoke(delegateFunction, gcnew array<Object^> { message });
				}
				else {
					txtOutput->AppendText(message + Environment::NewLine);
				}
			}
			catch (Exception ^ ex) {
				Console::WriteLine("appendOutput 處理委派時發生例外狀況 : " + ex->Message);
			}
		}

		void enableControl(bool start) {
			try {
				if (InvokeRequired) {
					ControlEnableDelegate^ delegateFunction = gcnew ControlEnableDelegate(this, &GeneratorForm::enableControl);
					Invoke(delegateFunction, gcnew array<Object^> { start });
				}
				else {
					txtThread->Enabled = !start;
					txtPuzzle->Enabled = !start;
					btnStart->Enabled = !start;
					btnStop->Enabled = start;
				}
			}
			catch (Exception ^ ex) {
				Console::WriteLine("enableControl 處理委派時發生例外狀況 : " + ex->Message);
			}
		}

		void outputEventHandler(String^ message) {
			appendOutput(message);
		}

		void taskFinishEventHandler() {
			enableControl(false);
		}

		void registerEvent() {
			LogEvent::generatorLogEvent += gcnew WriteGeneratorLog(this, &GeneratorForm::outputEventHandler);
			LogEvent::taskFinishEvent += gcnew AllTaskFinish(this, &GeneratorForm::taskFinishEventHandler);
		}
	public:
		void applyFont() {
			System::Drawing::Font^ display = ResourceHandler::getInstance()->getDisplayContentFont();
			labelOutput->Font = display;
			txtOutput->Font = display;

			labelThread->Font = display;
			txtThread->Font = display;

			labelPuzzle->Font = display;
			txtPuzzle->Font = display;

			btnStart->Font = display;
			btnStop->Font = display;
		}
	};
}