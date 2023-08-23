<?php

/*
	{xmakelink value="SUBMIT"}
	{xinput type="text" name="test1" value="aaa"}
	{xinput type="text" name="test2" value="bbb"}
	{/xmakelink}
*/

function smarty_function_xinput($params, $template)
{
	// TODO: readonly="readonly" input
	// $result = "
	// 	<input class=\"input_ok\" type=\"{$params["type"]}\" name=\"{$params["name"]}\" value=\"{$params["value"]}\">
	// ";
	$result = "
		<input class=\"%s\" type=\"{$params["type"]}\" name=\"{$params["name"]}\" value=\"{$params["value"]}\">
	";

// TODO FIX	
//<input class=\"%s\" %s type=\"{$params["type"]}\" name=\"{$params["name"]}\" value=\"{$params["value"]}\">
//sprintf($result, "$class", "{$params["readonly"]}");
	$class = "input_ok";

	if ($params["display_mode"] != "true") {
		if ($params["type"] == "numbers") {
			if (!preg_match("/^[0-9]+$/", $params["value"])) {
				$class = "input_error";
				$template->assign("is_form_error", true);
			}
		} elseif ($params["type"] == "time") {
			if (!preg_match("/^([0-1][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$/", $params["value"])) {
				$class = "input_error";
				$template->assign("is_form_error", true);
			}		
		} elseif ($params["type"] == "url") {
			if (!preg_match("/^(https?|ftp)(:\/\/[-_.!~*\'()a-zA-Z0-9;\/?:\@&=+\$,%#]+)$/", $params["value"])) {
				$class = "input_error";
				$template->assign("is_form_error", true);
			}				
		} elseif ($params["type"] == "text") {
			if (!preg_match("/^.+$/", $params["value"])) {
				$class = "input_error";
				$template->assign("is_form_error", true);
			}
		}
	}

	// final class setup
	if($params['class']) {
		$class .= " ".$params['class'];
	}
	return sprintf($result, "$class");
}
