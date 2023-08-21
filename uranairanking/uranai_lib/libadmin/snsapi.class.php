<?php

//======================================================================
// BASE
class SnsAPI {

	public $test_mode = false;

	// 使い方：
	// self::$log->add(...);
	public static $log;

	static function setLogObject($log_) {
		self::$log = $log_;
	}

	function login() {
		$this->log("Login() TODO");
	}

	protected function log($msg_) {
		print "LOG:".$msg_.PHP_EOL;
	}

	protected function error($err_) {
		print "ERR:".$err_.PHP_EOL;
	}

	protected function debug($var_) {
		if($this->test_mode) {
			print_r($var_);
			print PHP_EOL;
		}
	}

	// to be overriden
	protected function publish($msg_) {
		$this->log("publish() TODO");
	}
}
