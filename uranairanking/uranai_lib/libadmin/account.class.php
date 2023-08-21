<?php
/*
 * User Account logic
 * @date 2016-02-26
 */

class Account {

	/**
	 * account activation
	 *
	 * @author Azet
	 * @param array $user_
	 * @return bool
	 */
	static function sendActivateEmail($user_) {
		global $smarty;

		$smarty->assign('host', $_SERVER['SERVER_NAME']);

		$smarty->assign('user_activate', $user_);
		$body = $smarty->fetch("account-activate-mail.tpl");
		$mail = new mail();
		$mail->set_encoding("utf-8");
		$ok = $mail->send(
			MAIL_SENDER_EMAIL,
			MAIL_SENDER_NAME,
			$user_['email'],
			ACCOUNT_ACTIVATE_SUBJECT,
			$body);

		return $ok;
	}


	/**
	 * account data created/changed
	 *
	 * @author Azet
	 * @param array $user_
	 * @param bool $new_ true if new user, false if modification
	 * @return bool
	 */
	static function sendAccountDataEmail($user_, $new_=false) {
		//global $smarty;	//del okabe 2016/07/01
		global $smarty, $prefectureOptions;	//add okabe 2016/07/01

		$smarty->assign('host', $_SERVER['SERVER_NAME']);
		$smarty->assign('account_new', $new_);

		$smarty->assign('selected_notification', $user_['notifyMailSw']);		//add okabe 2016/06/20

		//add okabe start 2016/07/01
		$user_prefecture = $user_['prefecture'];
		$prefecture_name = "";
		if (intval($user_prefecture) > 0) {
			if (array_key_exists($user_prefecture, $prefectureOptions)) {
				$prefecture_name = $prefectureOptions[$user_prefecture];
			}
		}
		$smarty->assign('user_prefecture', $prefecture_name);
		//add okabe end 2016/07/01

		$smarty->assign('user_data', $user_);
		$body = $smarty->fetch("account-data-mail.tpl");
		$mail = new mail();
		$mail->set_encoding("utf-8");
		$ok = $mail->send(
			MAIL_SENDER_EMAIL,
			MAIL_SENDER_NAME,
			$user_['email'],
			ACCOUNT_DATA_SUBJECT,
			$body);

		return $ok;
	}


	/**
	 * password recovery
	 *
	 * @author Azet
	 * @param array $user_
	 * @paran string $newPass_
	 * @return bool
	 */
	static function sendPasswordEmail($user_, $newPass_) {
		global $smarty;

		$smarty->assign('host', $_SERVER['SERVER_NAME']);

		// data
		$smarty->assign('user_data', $user_);
		$smarty->assign('newpassword', $newPass_);

		$body = $smarty->fetch("account-password-mail.tpl");
		$mail = new mail();
		$mail->set_encoding("utf-8");
		$ok = $mail->send(
			MAIL_SENDER_EMAIL,
			MAIL_SENDER_NAME,
			$user_['email'],
			ACCOUNT_PASSWORD_SUBJECT,
			$body);

		return $ok;
	}


	/**
	 * Sets the current user ID in a cookie for later use
	 *
	 * @author Azet
	 * @param int $user_id_
	 */
	static function userSet($user_id_) {
		return setcookie('user', $user_id_, time()+COOKIE_ACCOUNT_PERIOD, '/');
	}


	static function userUnset() {
		unset($_COOKIE['user']);
		setcookie('user', null, -1, '/');
	}



	/**
	 * returns logged in user id (if any)
	 *
	 * @author Azet
	 * @return int or null
	 */
	static function userGetId() {
		return ($_COOKIE['user'])?$_COOKIE['user']:null;
	}


	/**
	 * returns user infos
	 *
	 * @author Azet
	 * @param int $user_id_
	 * @return array or null if error
	 */
	static function userInfos($user_id_=null) {
		global $conn;
		// false: not loaded
		// null: error
		// array: data availalbe
		static $user_data = false;

		// キャッシュ？
		if($user_data!==false) {
			//print "found data!";
			return $user_data;
		}

		if($user_id_) {
			// パラメータ？
			$user_id = $user_id_;
		 } else {
			// logged?
			$user_id = self::userGetId();
		}

		if($user_id) {
			$sql = "SELECT *
				FROM `users`
				WHERE `user_id`=$user_id
				AND `is_delete`=0
				LIMIT 1";
			$rs = mysqli_query($conn, $sql);
			if($rs && $rs->num_rows==1) {
				$user_data = $rs->fetch_assoc();
				$user_data['notification'] = array(
					'sunday' => $user_data['notification0']
					,'monday' => $user_data['notification1']
					,'tuesday' => $user_data['notification2']
					,'wednesday' => $user_data['notification3']
					,'thursday' => $user_data['notification4']
					,'friday' => $user_data['notification5']
					,'saturday' => $user_data['notification6']
				);
				$user_data['notificationHolidays'] = ($user_data['notificationHolidays']=='1')?'YES':'NO';
				$user_data['notificationSw'] = $user_data['notificationSw'];		//add okabe 2016/06/20
			}
		}

		return $user_data;
	}
	
}


/**
 * returns user birthday
 *
 * @author Azet
 * @param int $user_id_
 * @return ユーザーの星座番号を取得
 */
function get_user_bd($user_id_) {
	global $conn;
	$sql = "SELECT `birthday`
		FROM `users`
		WHERE `is_delete`=0 
		AND `notificationSw`=1 
		AND user_id = ".$user_id_;
		
	$user_bd_ = mysqli_query($conn, $sql);
	$user_bd = $user_bd_->fetch_assoc();
	$user_bd = getStarFromBirthday($user_bd['birthday']);
	return $user_bd;
}

