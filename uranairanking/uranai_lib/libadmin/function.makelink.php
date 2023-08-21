<?php

/**
 * any parameter can be added for hidden input
 *
 * @author Azet
 * @param array $params
 * @param string? $template
 */
function smarty_function_makelink($params, $template)
{
	$class="";
	if($params['class']) {
		$class = $params['class'];
	}

	$result = "<form class=\"button\" action=\"index.php\" method=\"POST\">";

	foreach($params as $k => $v) {
		// skipp
		if($k==='class') continue;
		// do
		$result .= "<input type=\"hidden\" name=\"$k\" value=\"$v\">";
	}

	$result .= "<input type=\"submit\" value=\"{$params["value"]}\" class='$class'>
		</form>
	";

	return $result;
}
