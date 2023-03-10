<?PHP
//	商品表示・非表示選択
function goods_4() {
include './array.inc';
global $main,$goods,$PHP_SELF,$maker,$bunrui,$s_goods,$s_size,$display,$view,$view_s,$view_e,$page,$views,$dis,$check,$max,$HTTP_POST_VARS,$stock;
	global $stock;
	include "../../cone.inc";

	while (list($key,$value) = each($HTTP_POST_VARS)) {
##		if (ereg("[0-9]_x","$key")) {
		if (preg_match("/[0-9]_x/","$key")) {
##			$key = ereg_replace("_x","",$key);
			$key = preg_replace("/_x/","",$key);
			if ($value <= 39) { $value = 2; } else { $value = 1; }
			$dis = "$key:$value";
		}
	}

	if ($check && $dis) {
##		$disp = split("\:",$dis);
		$disp = explode("\:",$dis);
		$sql  = "update list set".
				" display='$disp[1]',".
				" update_time=now()".
				" where list_num='$disp[0]';";
		$sql1 = pg_exec($conn_id,$sql);
	}

	if ($view) {

		$where = "";
		if ($maker) { $where .= " AND list.maker_num='".$maker."'"; }
		if ($bunrui) { $where .= " AND list.class_m='".$bunrui."'"; }
		if ($display) { $where .= " AND list.display='".$display."'"; }
		if ($stock == 1) {
			$where .= " AND goods.stock>'0'";
		} elseif ($stock == 2) {
			$where .= " AND goods.stock='0'";
		}

	if ($maker && $bunrui && $s_goods) { $where .= " AND goods.goods='".$s_goods."'"; }
	if ($s_goods && $s_size) { $where .= " AND goods.size = '".$s_size."'"; }

		$max = 0;
		$sql  = "SELECT count(list.*) AS count FROM list list".
				" LEFT JOIN goods goods ON goods.pluid=list.pluid".
				" LEFT JOIN maker_list_view maker ON maker.maker_num=list.maker_num".
				" WHERE list.state!='1'".
				" AND goods.pluid IS NOT NULL".
				$where.
				";";
//echo $sql."<br>\n";
		if ($result = pg_query($conn_id, $sql)) {
			$list = pg_fetch_array($result);
			//$max = $list['count'];	//	del ookawara 2014/01/21
			$count = $list['count'];	//	add ookawara 2014/01/21
		}

		$views = $VIEW_NUM[$view];
		//$page_all = ceil($max / $views);	//	del ookawara 2014/01/21
		$page_all = ceil($count / $views);	//	add ookawara 2014/01/21
		if (!$page) { $page = 1; }

		//	add ookawara 2014/01/21 start
		$start = $views * ($page-1);
		$end = $views;
		$limit = " OFFSET ".$start." LIMIT ".$end;
		$max = $count - 1;
		$view_s = $views * ($page-1);
		$view_e = ($views * $page)-1;
		if ($view_e > $max) { $view_e = $max; }
		//	add ookawara 2014/01/21 end

		//if ($max < 1) {	//	del ookawara 2014/01/21
		if ($count < 1) {	//	add ookawara 2014/01/21
			echo "登録されております商品はありません。<br><br>\n";
			return;
		}
	}

	//if ($view && $max > 0) {	//	del ookawara 2014/01/21
	if ($view && $count > 0) {	//	add ookawara 2014/01/21
		echo <<<ALPHA
表示を切り替えたい商品の<BR>
表示にしたい場合は表示の部分をクリック。<BR>
非表示にしたい場合は、非表示の部分をクリックして下さい。<BR>
登録数： $max Page : $page / $page_all <BR>
<form action='$PHP_SELF' method='POST'>
<input type='hidden' name='check' value='1'>
<input type="hidden" name="main" value="$main">
<input type="hidden" name="goods" value="$goods">
<input type="hidden" name="maker" value="$maker">
<input type="hidden" name="bunrui" value="$bunrui">
<input type="hidden" name="display" value="$display">
<input type="hidden" name="view" value="$view">
<input type="hidden" name="main_r" value="$main">
<input type="hidden" name="goods_r" value="$goods">
<input type="hidden" name="s_goods" value="$s_goods">
<input type="hidden" name="s_size" value="$s_size">
<input type="hidden" name="stock" value="$stock">
<input type="hidden" name="page" value="$page">
<table border="0" cellpadding="3" cellspacing="2" bgcolor='#999999'>
    <tr bgcolor='#cccccc'  align='center'>
      <td>商品番号</td>
      <td>表示切替</td>
      <td>ジャンコード</td>
      <td>商品名</td>
      <td>メーカー名</td>
      <td>カラー</td>
      <td>サイズ</td>
      <td>在庫数</td>
    </tr>

ALPHA;

		$offset = ($page - 1) * $views;
		$limit_max = $offset + $views;
		//if ($max < $limit_max) {		//	del ookawara 2014/01/21
		//	$limit = $max % $views;		//	del ookawara 2014/01/21
		if ($count < $limit_max) {		//	add ookawara 2014/01/21
			$limit = $count % $views;	//	add ookawara 2014/01/21
		} else {
			$limit = $views;
		}
		$limit_num = " LIMIT $limit";
		if ($offset != 0) {
			$limit_num .= " OFFSET $offset";
		}

		$sql  = "SELECT".
					" list.list_num, list.display, list.goods_name, list.color, list.size,".
					" list.pluid, list.maker_num,".
					" goods.stock,".
					" maker.maker_name".
				" FROM list list" .
				" LEFT JOIN goods goods ON goods.pluid=list.pluid".
				" LEFT JOIN maker_list_view maker ON maker.maker_num=list.maker_num".
				" WHERE list.state!='1'".
				" AND goods.pluid IS NOT NULL".
				$where.
				" ORDER BY list.list_num".
				$limit_num.
				";";
//echo $sql."<br>\n";
		if ($result = pg_query($conn_id, $sql)) {
			WHILE ($list = pg_fetch_array($result)) {
				$list_num_ = $list['list_num'];
				$display_ = $list['display'];
				$goods_name_ = $list['goods_name'];
				$color_ = $list['color'];
				$size_ = $list['size'];
				$pluid_ = $list['pluid'];
				$maker_num_ = $list['maker_num'];
				$stock_ = $list['stock'];
				$maker_name_ = $list['maker_name'];

				if ($stock_ == "") { $stock_ = 0; }

				if ($display_ == 1) {
					$dis = "$list_num_:2";
					$dis_pic = "./images/hihyouji.gif";
				} else {
					$dis = "$list_num_:1";
					$dis_pic = "./images/hyouji.gif";
				}
				if ($color_ == "") { $color_ = "--"; }
				if ($size_ == "") { $size_ = "--"; }

				echo <<<ALPHA
    <tr bgcolor='#ffffff'>
      <td align='right'>$list_num_</td>
      <td align='center'><input type='image' name='$list_num_' value='$dis' src='$dis_pic'></td>
      <td>$pluid_</td>
      <td>$goods_name_</td>
      <td>$maker_name_</td>
      <td align='center'>$color_</td>
      <td align='center'>$size_</td>
      <td align='center'>$stock_</td>
    </tr>
ALPHA;
			}
		}

		echo <<<ALPHA
</table>
</form>

ALPHA;

		//ページ処理
		if ($page_all > $page || $page >= 2) { next_p_g_1(); }

	}

	pg_close($conn_id);

}
?>
