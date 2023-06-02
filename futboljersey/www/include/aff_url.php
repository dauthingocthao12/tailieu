<?PHP
/*

	ネイバーズスポーツ　アフィリエイトURL表示

*/
function aff_url($CHECK) {

	$html = "";

	$flag = 0;
	if ($_SESSION['idpass']) {
		list($email,$pass,$check,$af_num) = explode("<>",$_SESSION['idpass']);
		if ($af_num) { $flag = 1; }
	}

	if ($flag == 1) {
		if ($CHECK) {
			if ($CHECK['page']) { $p_url = $CHECK['page']; }
			elseif ($CHECK['g']) { $p_url = $CHECK['g']; }
			elseif ($CHECK['l']) {
				$p_url  = "l" . sprintf("%02d",$CHECK['main']);
##				$s = eregi_replace("[^0-9]","",$CHECK['s']);
				$s = preg_replace("/[^0-9]/i","",$CHECK['s']);
				$p_url .= sprintf("%02d",$s);
##				$l = eregi_replace("[^0-9]","",$CHECK['l']);
				$l = preg_replace("/[^0-9]/i","",$CHECK['l']);
				$p_url .= sprintf("%02d",$l);
			}
			elseif ($CHECK['s']) {
				$p_url  = "s" . sprintf("%02d",$CHECK['main']);
##				$s = eregi_replace("[^0-9]","",$CHECK['s']);
				$s = preg_replace("/[^0-9]/i","",$CHECK['s']);
				$p_url .= sprintf("%02d",$s);
			}
			elseif ($CHECK['main']) {
				$p_url  = sprintf("%02d",$CHECK['main']);
			}
			$p_url  .= "/";
		}

		$html .= "<div class=\"aff_url box-outline\">\n";
		$html .= "  <div class='box-title'>アフィリエイトページリンク作成</div>\n";
		$html .= "  <a href=\""."/aff_link/".$p_url."\" class=\"box-content\">作成する</a>\n";
		$html .= "</div>\n";

	}

	return $html;

}
?>
