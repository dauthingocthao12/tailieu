<?PHP
//	アフィリエイトURL表示
function aff_url($CHECK) {
global $URL;

	$html = "<center>\n";

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
				$s = preg_replace("/[^0-9]/","",$CHECK['s']);
				$p_url .= sprintf("%02d",$s);
##				$l = eregi_replace("[^0-9]","",$CHECK['l']);
				$l = preg_replace("/[^0-9]/","",$CHECK['l']);
				$p_url .= sprintf("%02d",$l);
			}
			elseif ($CHECK['s']) {
				$p_url  = "s" . sprintf("%02d",$CHECK['main']);
##				$s = eregi_replace("[^0-9]","",$CHECK['s']);
				$s = preg_replace("/[^0-9]/","",$CHECK['s']);
				$p_url .= sprintf("%02d",$s);
			}
			elseif ($CHECK['main']) {
				$p_url  = sprintf("%02d",$CHECK['main']);
			}
			$p_url  .= "/";
		}

		$html .= <<<WAKABA
      <form>
      <table width="750px">
          <tr>
            <th class="cate1">アフィリエイトページリンク作成</th>
            <td class="cate3">
            <a href="/aff_link/{$p_url}">作成する</a>
            </td>
          </tr>
      </table>
      </form>

WAKABA;
	}
/*
	$cautions_file = "foot_cautions.htm";
	if (file_exists($cautions_file)) {
		$html .= file_get_contents($cautions_file);
	}
*/
	//	メーカー一覧
	$maker_file = "./sub/maker_list.inc";
	if (file_exists($maker_file)) {
		include("$maker_file");
		$html .= maker_list();
	}

	$html .= "</center>\n";

	return $html;

}
?>
