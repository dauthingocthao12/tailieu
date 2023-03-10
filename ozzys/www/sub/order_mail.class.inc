<?php

class OrderMail {


	// save params for later use
	static function make_params($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr) {
		return array(
			'sender_email' => $send_email,
			'sender_name'  => $send_name,
			'_unused1'     => $mail_get,
			'_unused2'     => $get_email,
			'email'        => $email,
			'subject'      => $subject,
			'msg'          => $msr
		);
	}


	/**
	 * after different process, send to client
	 *
	 * @param array $params_ (make_params() のリターン値)
	 * @return array(bool, array) メールファンクションのリターン値＋メッセージ
	 */
	static function sendToClient($params_) {
		list($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr) = array_values($params_);
		$ERROR = array();

		$check = send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);
		if ($check) {
			$ERROR[] = "ご注文確認メールが送信できません。メールアドレスをご確認してください。";
		}

		return array($check, $ERROR);
	}


	/**
	 * after different process, send to admin
	 *
	 * @param array $params_ (make_params() のリターン値)
	 * @return array(bool, array) メールファンクションのリターン値＋メッセージ
	 */
	static function sendToAdmin($params_) {
		list($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr) = array_values($params_);
		$ERROR = array();

		$check = send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);
		if ($check) {
			$ERROR[] = "ご注文メールが送信できません。メールアドレスをご確認してください。";
		}

		return array($check, $ERROR);
	}

}