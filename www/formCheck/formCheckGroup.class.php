<?php

class FormCheckGroup {

	private $fields = array();
	private $errors = array();


	/**
	 * フォーム毎に入力フィールドを追加
	 * @param FormCheckRule $field_
	 */
	function addField($field_) {
		$this->fields[] = $field_;
	}


	/**
	 * フォームのデータは正しいか確認
	 * @param array $input_ the input form, usually $_POST
	 * @return bool
	 */
	function isValid($input_) {
		$this->errors = array();	// reset

		for($i=count($this->fields)-1; $i>=0; --$i) {
			$field = $this->fields[$i];
			$name = $field->nameInForm;
			if(!$field->isValid($input_[$name])) {
				$this->errors[$name] = $field->error;
			}
		}

		return count($this->errors)===0;
	}


	/**
	 * 登録されたフィールドを出力
	 * @param $fieldName_
	 * @return formCheckRule Object, 問題の場合はnull
	 */
	function getField($fieldName_) {
		$field = null;

		for($i=count($this->fields)-1; $i>=0; --$i) {
			$field_search = $this->fields[$i];
			$name = $field_search->nameInForm;
			if($name===$fieldName_) {
				$field = $field_search;
				break;
			}
		}

		return $field;
	}


	/**
	 * 登録されたフィールドを出力
	 * @return array of fields
	 */
	function getFields() {
		$fields = array();
		foreach($this->fields as $field) {
			$fields[] = $field->nameInForm;
		}
		return $fields;
	}

	/**
	 * isValid()の時にエラーがあったら、ここで出力する
	 * @return array [k => v] kはフィールドエントリー、vはエラーメッセージ
	 */
	function getErrors() {
		return $this->errors;
	}

}
