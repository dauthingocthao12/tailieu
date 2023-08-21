<?php
// LIBS
// http://github.com/j7mbo/twitter-api-php
require_once dirname(__FILE__)."/TwitterAPIExchange.php";

//======================================================================
// TWITTER
class TwitterAPI extends SnsAPI {

	// change those constants by attributes and assign them in constructor should be the solution
	private $APITwitterUpdateURL = "https://api.twitter.com/1.1/statuses/update.json";

	// auth
	private $ApiSettings = array(
		'oauth_access_token' => "4859029857-F1mc0RNnpeT8CpulcJZ7jsjgOqVh0HdHCLNWybh",
		'oauth_access_token_secret' => "FCdgO0nd3DXh3cdhoh4ppNURjUFlj6KzfvKsl4djxeIRL",
		'consumer_key' => "lK6XwxXP6rWSckxcW6ka261US",
		'consumer_secret' => "p2MFSoJKWcxfHfZ3tJUrjKMB9qon31auWd1qr7znSfUyB6jXTE"
	);	// append token post data during the requests

	// for ssl and curl
	private $CurlOptions = array(
		CURLOPT_SSL_VERIFYPEER => false
	);

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
			$msg = preg_replace("/\n/", ' ', "TEST TwitterAPI->publish(): $msg_");
			self::$log->add("TWITTER API", $msg);
			print $msg;
			return true;
		}

		// data
		$postfields = array(
			'status' => $msg_
		);

		// API call
		$twitter = new TwitterAPIExchange($this->ApiSettings);
		$result_json = $twitter->buildOauth($this->APITwitterUpdateURL, 'POST')
			->setPostfields($postfields)
			->performRequest(true, $this->CurlOptions);

		$this->debug($result_json);
		$result = json_decode($result_json);

		//$this->debug($result);
		if($result->created_at) {
			print $log_msg = "OK ツイット作成できました";
			self::$log->add("TWITTER API", $log_msg);
			return true;
		}
		else {
			print $log_msg = "ERR ツイット作成できませんでした: ".$result->errors[0]->message;
			self::$log->add("TWITTER API", $log_msg);
			return false;
		}
	}

}


/* sample returned message >>> 
{
	"created_at":"Wed Feb 03 09:26:18 +0000 2016",
	"id":694814156860461056,
	"id_str":"694814156860461056",
	"text":"Test Message!",
	"source":"\u003ca href=\"http:\/\/uranairanking.jp\" rel=\"nofollow\"\u003euranairankingjp\u003c\/a\u003e",
	"truncated":false,
	"in_reply_to_status_id":null,
	"in_reply_to_status_id_str":null,
	"in_reply_to_user_id":null,
	"in_reply_to_user_id_str":null,
	"in_reply_to_screen_name":null,
	"user":{
		"id":4859029857,
		"id_str":"4859029857",
		"name":"12\u661f\u5ea7\u5360\u3044\u30e9\u30f3\u30ad\u30f3\u30b0",
		"screen_name":"UranaiRankingJp",
		"location":"",
		"description":"\u5404\u30b5\u30a4\u30c8\u306e12\u661f\u5ea7\u30e9\u30f3\u30ad\u30f3\u30b0\u3092\u516c\u958b\u3057\u3066\u307e\u3059\u3002",
		"url":"https:\/\/t.co\/irL6GjrcfT",
		"entities":{
			"url":{
				"urls":[
					{
						"url":"https:\/\/t.co\/irL6GjrcfT",
						"expanded_url":"http:\/\/uranairanking.jp\/",
						"display_url":"uranairanking.jp",
						"indices":[
							0,
							23
						]
					}
				]
			},
			"description":{
				"urls":[]
			}
		},
		"protected":false,
		"followers_count":2,
		"friends_count":0,
		"listed_count":0,
		"created_at":"Fri Jan 29 05:11:23 +0000 2016",
		"favourites_count":0,
		"utc_offset":null,
		"time_zone":null,
		"geo_enabled":false,
		"verified":false,
		"statuses_count":1,
		"lang":"ja",
		"contributors_enabled":false,
		"is_translator":false,
		"is_translation_enabled":false,
		"profile_background_color":"000000",
		"profile_background_image_url":"http:\/\/abs.twimg.com\/images\/themes\/theme1\/bg.png",
		"profile_background_image_url_https":"https:\/\/abs.twimg.com\/images\/themes\/theme1\/bg.png",
		"profile_background_tile":false,
		"profile_image_url":"http:\/\/abs.twimg.com\/sticky\/default_profile_images\/default_profile_3_normal.png",
		"profile_image_url_https":"https:\/\/abs.twimg.com\/sticky\/default_profile_images\/default_profile_3_normal.png",
		"profile_link_color":"9266CC",
		"profile_sidebar_border_color":"000000",
		"profile_sidebar_fill_color":"000000",
		"profile_text_color":"000000",
		"profile_use_background_image":false,
		"has_extended_profile":false,
		"default_profile":false,
		"default_profile_image":true,
		"following":false,
		"follow_request_sent":false,
		"notifications":false
	},
	"geo":null,
	"coordinates":null,
	"place":null,
	"contributors":null,
	"is_quote_status":false,
	"retweet_count":0,
	"favorite_count":0,
	"entities":{
		"hashtags":[],
		"symbols":[],
		"user_mentions":[],
		"urls":[]
	},
	"favorited":false,
	"retweeted":false,
	"lang":"en",
	"ext":{
		"stickerInfo":{
			"r":{
				"err":{
					"code":402,
					"message":"ColumnNotFound"
				}
			},
			"ttl":-1
		}
	}
}
 <<< */
