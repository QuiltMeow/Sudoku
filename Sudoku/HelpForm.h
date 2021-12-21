#pragma once

#include "ResourceHandler.h"

namespace Sudoku {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class HelpForm sealed : public System::Windows::Forms::Form {
	public:
		HelpForm() {
			InitializeComponent();
		}
	protected:
		~HelpForm() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::Label^ labelIntroduction;
		System::Windows::Forms::Label^ labelIntroductionTitle;
		System::Windows::Forms::Label^ labelRuleTitle;
		System::Windows::Forms::Label^ labelRule;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����]�p�u��䴩�һݪ���k - �ФŨϥε{���X�s�边�ק�
		/// �o�Ӥ�k�����e�C
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(HelpForm::typeid));
			this->labelIntroduction = (gcnew System::Windows::Forms::Label());
			this->labelIntroductionTitle = (gcnew System::Windows::Forms::Label());
			this->labelRuleTitle = (gcnew System::Windows::Forms::Label());
			this->labelRule = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			//
			// labelIntroduction
			//
			this->labelIntroduction->AutoSize = true;
			this->labelIntroduction->Location = System::Drawing::Point(25, 40);
			this->labelIntroduction->Name = L"labelIntroduction";
			this->labelIntroduction->Size = System::Drawing::Size(293, 36);
			this->labelIntroduction->TabIndex = 1;
			this->labelIntroduction->Text = L"�ƿW�O�@�ؼƾ��޿�C���A�C���� 9 �� 9 �Ӯ�l�զ��A\r\n���a�ݭn�ھڮ�l���Ѫ��Ʀr���z�X��L��l���Ʀr�C\r\n�D�ش��ѳ̤� 17 �ӼƦr�ϱo�ѵ����D�u���@�ӵ���"
				L"�C";
			//
			// labelIntroductionTitle
			//
			this->labelIntroductionTitle->AutoSize = true;
			this->labelIntroductionTitle->Location = System::Drawing::Point(12, 9);
			this->labelIntroductionTitle->Name = L"labelIntroductionTitle";
			this->labelIntroductionTitle->Size = System::Drawing::Size(29, 12);
			this->labelIntroductionTitle->TabIndex = 0;
			this->labelIntroductionTitle->Text = L"����";
			//
			// labelRuleTitle
			//
			this->labelRuleTitle->AutoSize = true;
			this->labelRuleTitle->Location = System::Drawing::Point(12, 115);
			this->labelRuleTitle->Name = L"labelRuleTitle";
			this->labelRuleTitle->Size = System::Drawing::Size(29, 12);
			this->labelRuleTitle->TabIndex = 2;
			this->labelRuleTitle->Text = L"�W�h";
			//
			// labelRule
			//
			this->labelRule->AutoSize = true;
			this->labelRule->Location = System::Drawing::Point(25, 145);
			this->labelRule->Name = L"labelRule";
			this->labelRule->Size = System::Drawing::Size(418, 48);
			this->labelRule->TabIndex = 3;
			this->labelRule->Text = L"�C���� 9 �� 3 �� 3 �Ӫ��E�c��զ��C\r\n�C�@�C���Ʀr�����]�t 1 ~ 9�A����ʤ֡A�]���୫�ơC\r\n�C�@�檺�Ʀr�����]�t 1 ~ 9�A����ʤ֡A�]���୫��"
				L"�C\r\n�C�@�c (�ʶ½u��_�Ӫ��ϰ�) ���Ʀr�����]�t 1 ~ 9�A����ʤ֡A�]���୫�ơC\r\n";
			//
			// HelpForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(619, 231);
			this->Controls->Add(this->labelRule);
			this->Controls->Add(this->labelRuleTitle);
			this->Controls->Add(this->labelIntroductionTitle);
			this->Controls->Add(this->labelIntroduction);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->Name = L"HelpForm";
			this->Text = L"�W�h����";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &HelpForm::HelpForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	public:
		void applyFont() {
			System::Drawing::Font^ displayLargeTitle = ResourceHandler::getInstance()->getDisplayLargeTitleFont();
			System::Drawing::Font^ displayTitle = ResourceHandler::getInstance()->getDisplayTitleFont();
			labelIntroductionTitle->Font = displayLargeTitle;
			labelRuleTitle->Font = displayLargeTitle;

			labelIntroduction->Font = displayTitle;
			labelRule->Font = displayTitle;
		}
	private:
		System::Void HelpForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			Hide();
			e->Cancel = true;
		}
	};
}