<?php
class MastodonAPI extends SnsAPI {

	public $test_mode = false;
	//PAWOO用トークン
	private $access_token = 'b009c8e19828b23ce1fe00ce4f7b0be5a0a1ea50619d23c91438a4be8c3859db';
	
	/*
	* 投稿
	*
	* @param string $msg_　投稿内容
	* @param string $visibility 公開範囲 public,private,...
	*/
	
	function publish($msg_){
		//テストモード
		if($this->test_mode) {
			$msg = preg_replace("/\n/", ' ', "TEST MastodonAPI->publish(): $msg_");
			self::$log->add("MASTODON API", $msg);
			print $msg;
			return true;
		}
	
		//送信するデータ
		$data = array(
			'status' => $msg_,
			'visibility' => 'public'
		);
		
		$ch = curl_init();
		
		$options = array(
			CURLOPT_URL => 'https://pawoo.net/api/v1/statuses',
			CURLOPT_POST => true,
			CURLOPT_POSTFIELDS => http_build_query($data),
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_SSL_VERIFYPEER => false,
			CURLOPT_SSL_VERIFYHOST => false,
			CURLOPT_HTTPHEADER => array('Authorization: Bearer ' . $this->access_token)
		);

		curl_setopt_array($ch,$options);
		$result = curl_exec($ch);
		$result = json_decode($result);
		curl_close($ch);
		
		if($result->created_at) {
			print $log_msg = "OK トゥートを作成できました";
			self::$log->add("MASTODON API", $log_msg);
			return true;
		} else {
			print $log_msg = "ERR トゥートを作成できませんでした: " . $result->errors;
			self::$log->add("MASTODON API", $log_msg);
			return false;
		}
	}
}

/***

 マストドンAPIからのレスポンスの一例 var_dump()
 STRINGで返ってくるのでJSONオブジェクトか連想配列に変換して使います。

 成功:

string(1091) "{"id":26403009,"created_at":"2017-07-03T08:03:31.315Z","in_reply_to_id":null,"in_reply_to_account_id":null,"sensitive":null,"spoiler_text":"","visibility":"private","language":"ja","application":{"name":"myappl","website":null},"account":{"id":226536,"username":"azetkimura","acct":"azetkimura","display_name":"","locked":false,"created_at":"2017-05-18T07:23:11.041Z","followers_count":4,"following_count":0,"statuses_count":38,"note":"<p></p>","url":"https://mstdn.jp/@azetkimura","avatar":"https://media.mstdn.jp/images/accounts/avatars/000/226/536/original/328641132ac9e1f8.png","avatar_static":"https://media.mstdn.jp/images/accounts/avatars/000/226/536/original/328641132ac9e1f8.png","header":"/headers/original/missing.png","header_static":"/headers/original/missing.png"},"media_attachments":[],"mentions":[],"tags":[],"uri":"tag:mstdn.jp,2017-07-03:objectId=26403009:objectType=Status","content":"<p>メッセー
ジのテスト</p>","url":"https://mstdn.jp/@azetkimura/26403009","reblogs_count":0,"favourites_count":0,"reblog":null,"favourited":false
,"reblogged":false,"muted":false}"

 失敗:
 
string(54) "{"error":"アクセストークンが無効です。"}"

***/
