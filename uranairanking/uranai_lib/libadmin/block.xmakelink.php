<?php

/*
	{xmakelink value="SUBMIT"}
	{xinput type="text" name="test1" value="aaa"}
	{xinput type="text" name="test2" value="bbb"}
	{/xmakelink}
*/


function smarty_block_xmakelink($params, $content, &$smarty, &$repeat)
{
	// 終了タグでのみ出力します
	if(!$repeat){
		if (isset($content)) {
			$class = "";
			if($params['class']) {
				$class = 'class="'.$params['class'].'"';
			}
			#$content = preg_replace("/submit/", "text", $content);
			$content = "<form class=\"button\" action=\"index.php\" method=\"POST\">" . $content;
			$content = $content . "
				<input type=\"hidden\" name=\"mode\" value=\"{$params["mode"]}\">
				<input type=\"hidden\" name=\"action\" value=\"{$params["action"]}\">
				<input type=\"hidden\" name=\"id\" value=\"{$params["id"]}\">
				<center>
				<input type=\"submit\" value=\"{$params["value"]}\" $class>
				</center>
			";
			$content = $content . "</form>";
			return "$content";
		}
	}
}
