<?php
/**
 * Checking fields data
 *
 * @author Simon Cedric
 * @update 2015-07-23 
 */


class FormCheckRule {

	protected $nameToUser;	// 例：パスワード
	public $nameInForm;	// 例:passwd
	private $format;	// regexp
	// to customize if needed
	private $errorMessage = null;

	public $error = '';	// error message after trying to validate

	// default password validation regex (8 characters min with digits, lower and upper case letters
	static public $passwordFormat = '/^((?=.*\d+)(?=.*[a-z]+)(?=.*[A-Z]+))\w{8,}$/m';

	// basic default message
	static public $errorMessageInvalid = '[%s] が無効です';
	static public $errorMessageEmpty = '[%s] が必要です';


	/**
	 * コンストラクタ
	 * @param $name_to_user_ string displayed text in UI
	 * @param $name_in_form_ string inner name used in the logic
	 * @param $format_ string regex format
	 * @param $error_msg_ string A customized error message
	 * matches anything by default
	 * @return Object
	 */
	function __construct($name_to_user_, $name_in_form_, $format_='/.*/', $error_msg_='') {
		$this->nameToUser = $name_to_user_;
		$this->nameInForm = $name_in_form_;
		$this->format = $format_;
		if($error_msg_) {
			$this->errorMessage = $error_msg_;
		}
	}


	/**
	 * フィールドの値は正しいか確認
	 * @param string value to check for that field
	 * @return bool
	 */
	function isValid($value_) {
		$ok = false;

		// empty?
		if(!$value_) {
			if($this->errorMessage) {
				$this->error = sprintf($this->errorMessage, $this->nameToUser);
			}
			else {
				$this->error = sprintf(self::$errorMessageEmpty, $this->nameToUser);
			}
		}
		else {
			if(preg_match($this->format, $value_)) {
				$ok = true;
			}
			else {
				if($this->errorMessage) {
					$this->error = sprintf($this->errorMessage, $this->nameToUser);
				}
				else {
					$this->error = sprintf(self::$errorMessageInvalid, $this->nameToUser);
				}
			}
		}

		return $ok;
	}

}

class FormCheckRuleDateIso extends FormCheckRule {

	function __construct($name_to_user_, $name_in_form_) {
		parent::__construct($name_to_user_, $name_in_form_, "/^\d{1,4}-\d{2}-\d{2}$/");
	}

	function isValid($value_) {
		//var_dump($this);
		$ok = parent::isValid($value_);

		if($ok) {
			// checking values
			list($y, $m, $d) = explode('-', $value_);
			$y = (int)$y;
			$m = (int)$m;
			$d = (int)$d;
			$ok = checkdate($m, $d, $y);
		}

		if(!$ok) {
			$this->error = sprintf(self::$errorMessageInvalid, $this->nameToUser);
		}

		return $ok;
	}
}


class FormCheckRuleEmail extends FormCheckRule {

	function __construct($name_to_user_, $name_in_form_, $msg_='') {
		parent::__construct($name_to_user_, $name_in_form_, '', $msg_);
	}

	function isValid($value_) {
		$ok = filter_var($value_, FILTER_VALIDATE_EMAIL);
		if(!$ok) {
			$this->error = sprintf(self::$errorMessageInvalid, $this->nameToUser);
		}

		return $ok;
	}
}
