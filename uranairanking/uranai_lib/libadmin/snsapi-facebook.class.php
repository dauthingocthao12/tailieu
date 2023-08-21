<?php
// FACEBOOK
class FacebookAPI extends SnsAPI {

	// API
	private $graph_version = 'v2.7';

	// link to how to generate a never expiring page access token: http://stackoverflow.com/a/28418469/921796
	private $access_token = 'EAAYmBrl8eokBANoYEugw0nnc8EdDa68EdDIX2tPKnrGduc1eVtiMXeHxbti6ZCKTKgIXrPZBwUD3ufrZC5GOO6CatAHQBnH5KMGEsji0XR3xOQZAufLptbQ4NtYXtP2fqlAejPt4ztVjjbzhUoZBx5k55MAxEs17Ko46runZANDwZDZD';
	//上記のトークンは大河原さんのアカウントで作られました。

	// 規定は本番モード
	public $test_mode = false;


	/**
	 * 送信
	 *
	 * @author Azet
	 * @param string $msg_
	 * @return bool
	 */
	function publish($msg_) {

		// debug mode
		if($this->test_mode) {
			$msg = preg_replace("/\n/", ' ', "TEST FacebookAPI->publish(): $msg_");
			self::$log->add("FACEBOOK API", $msg);
			print $msg;
			return true;
		}

		// api URI
		$graph_url= "https://graph.facebook.com/".$this->graph_version."/me/feed";
		// POST データ
		$postData =   "&message=" . urlencode($msg_)
			. "&link=" . urlencode('https://uranairanking.jp')
			. "&access_token=" . $this->access_token;

		$ch = curl_init();

		curl_setopt($ch, CURLOPT_URL, $graph_url);
		curl_setopt($ch, CURLOPT_HEADER, 0);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $postData);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);

		$result_json = curl_exec($ch);
		//var_dump($result_json);
		$this->debug($result_json);

		curl_close($ch);

		// return 確認
		$result = json_decode($result_json);
		if(!$result->id) {
			//var_dump($result);
			$error_obj = $result->error;
			$error = $error_obj->message;

			print $log_msg = "ERR ポスト作成できませんでした: ".$error;
			self::$log->add("FACEBOOK API", $log_msg);
			return false;
		}
		else {
			print $log_msg = "OK ポスト作成できました";
			self::$log->add("FACEBOOK API", $log_msg);
			return true;
		}
	}
}
