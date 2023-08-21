<?php

function login_check() {
	if ($_POST) {
		//if ($_POST["user"] == USER && $_POST["pass"] == PASS && !$_SESSION["user"]) {	//del okabe 2016/05/25
		if ($_POST["user"] == USER && md5($_POST["pass"]) == PASS && !$_SESSION["user"]) {	//add okabe 2016/05/25 pwのmd5でのチェック化
			$_SESSION["user"] = $_POST["user"];
			$result = array(
				/*"template_page" => "templates/home-go.tpl",*/
				"status" => "OK",
				"message" => "認証しました。"
			);
		} else {
			$result = array(
				"message" => "ユーザ名またはパスワードが違います。"
			);
		}
	} else {
		$result = array(
			"message" => "ユーザ名とパスワードを入力してください。"
		);
	}
	return $result;
}
