<?php

/**
 * CommonValidator
 * 汎用バリデーションクラス
 * 
 * 使い方:
 * uranai_lib/_tests/validator_test.php
 * 
 * @author azet
 */
class CommonValidator
{

	/**
	 * チェック対象項目
	 * @var array
	 */
	private $fields = [];

	/**
	 * バリデーションルール(関数の配列)
	 * @var array
	 */
	private $rules = [];

	/**
	 * エラー配列
	 * @var array
	 */
	private $errors = [];

	/**
	 * __construct
	 *
	 * @param array $fields
	 */
	public function __construct($fields = [])
	{
		$this->fields = $fields;
	}

	/**
	 * フィールドのキーバリューを追加する
	 * @param array $fields
	 */
	public function val($fieldKey, $fieldValue)
	{
		$this->fields[$fieldKey] = $fieldValue;
		return $this;
	}

	/**
	 * バリデーションルールを追加する
	 *
	 * @param string $field 項目名
	 * @param string $errorMessage 値が不正だった際ののメッセージ
	 * @param Callable $validatorFunc バリデーションに用いる関数
	 * @param mixed $validatorFuncParams バリデーションに用いる関数の引数（オプショナル）
	 *
	 * @return $this
	 */
	public function rule($field, $errorMessage, $validatorFunc, $validatorFuncParams = null, $rule_id = null)
	{
		if (!isset($this->fields[$field])) {
			throw new Exception('Field does not exist. [' . $field . ']', 1);
		}
		$self = $this;
		if (!is_callable($validatorFunc, true)) {
			throw new Exception('Function not found. [' . $validatorFunc . ']', 2);
		}

		$id = uniqid();
		if ($rule_id) {
			$id = $rule_id;
		}
		$rule = function () use ($self, $field, $errorMessage, $validatorFunc, $validatorFuncParams, $id) {
			if ($validatorFuncParams) {
				$ret = $validatorFunc($self->fields[$field], $validatorFuncParams);
			} else {
				$ret = $validatorFunc($self->fields[$field]);
			}
			if (!$ret) {
				$this->errors[$field][$id] = $errorMessage;
			}
			return $ret;
		};
		$this->rules[$field][$id] = $rule;
		return $this;
	}

	/**
	 * ある項目に対して必須ルールを追加する
	 *
	 * @param string $field 項目名
	 * @param string $errorMessage 値が不正だった際ののメッセージ
	 *
	 * @return $this
	 */
	public function required($field, $errorMessage)
	{
		$this->rule($field, $errorMessage, function ($v) {
			return !empty($v);
		}, null, 'required');
		return $this;
	}

	/**
	 * ある項目に対して正規表現のマッチでルールを追加する
	 *
	 * @param string $field 項目名
	 * @param string $errorMessage 値が不正だった際ののメッセージ
	 * @param string $regex 正規表現パターン（デリミタ含む） ex. "/[a-z]/"
	 *
	 * @return $this
	 */
	public function regex($field, $errorMessage, $regex)
	{
		$this->rule($field, $errorMessage, function ($v) use ($regex) {
			return preg_match($regex, $v) === 1;
		});
		return $this;
	}
	/**
	 * セットされたルールに対して整合性チェックを実行する
	 *
	 * @return boolean 1つでも不正があった場合false|すべて合格した場合true
	 */
	public function validate()
	{
		$valid = false;
		//ルールごとのチェック
		foreach ($this->rules as $rules) {
			foreach ($rules as $rule) {
				$valid = $valid or $rule();
			}
		}
		return $valid;
	}

	/**
	 * 各項目のバリデーション結果に対するエラーメッセージをを取得する
	 * 「入力必須(required)」エラーとその他の複数のエラーがある場合は、そのフィールドに対しては入力必須エラーのみを返す
	 * からの項目に対して値の形式の判定まで行ってしまうと、エラー内容が冗長になるため
	 *
	 * @return array
	 */
	public function getErrors()
	{
		$errors = [];
		foreach ($this->errors as $key => $errs) {
			$has_required_err = in_array('required', array_keys($errs));
			if ($has_required_err) {
				$errors[$key]['required'] = $errs['required'];
			} else {
				$errors[$key] = $errs;
			}
		}
		return $errors;
	}

	/**
	 * 各項目のバリデーション結果に対するエラーメッセージをを取得する(全件取得)
	 *
	 * @return array
	 */
	public function getErrorsRaw()
	{
		return $this->errors;
	}

	/**
	 * 文字列が全角カタカナかをチェックする
	 *
	 * @param string $val
	 * @static
	 *
	 * @return boolean
	 */
	static function isZenkakuKatakana($val)
	{
		return preg_match("/\A[ァ-ヴー]+\z/u", $val) === 1;
	}

	/**
	 * 文字列がEメールアドレスかどうかをチェックする
	 *
	 * @param string $val
	 * @static
	 *
	 * @return boolean
	 */
	static function isEmailAdress($val)
	{
		return preg_match("/^[\w\-\.]+@([\w\-]+\.)+[\w\-]{2,4}$/", $val) === 1;
	}

	/**
	 * 文字列がある長さ以下かチェックする
	 *
	 * @param string $val
	 * @static
	 *
	 * @return boolean
	 */
	static function lengthMax($val, $len)
	{
		return mb_strlen($val, 'UTF-8') <= $len;
	}
}
