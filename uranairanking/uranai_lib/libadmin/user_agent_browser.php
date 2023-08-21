<?php
function browser_name($user_agent){
	if(preg_match("/msie|Trident/", $user_agent)){
		$user_agent_browser = "IE";
	}else if(preg_match("/Edge/", $user_agent)){
		$user_agent_browser = "Edge";
	}else if(preg_match("/Chrome/", $user_agent)){
		$user_agent_browser = "chrome";
	}else if(preg_match("/CriOS/", $user_agent)){
		$user_agent_browser = "apple_chrome";
	}else if(preg_match("/Safari/", $user_agent)){
		$user_agent_browser = "safari";
	}else if(preg_match("/Firefox/", $user_agent)){
		$user_agent_browser = "firefox";
	}else if(preg_match("/Opera/", $user_agent)){
		$user_agent_browser = "opera";
	}else{
		$user_agent_browser = "other";
	}return $user_agent_browser;
}
?>
