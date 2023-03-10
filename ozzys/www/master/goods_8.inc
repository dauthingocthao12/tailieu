<?PHP
//	商品詳細削除
function goods_8() {
global $view,$mode;

if ($view) {
	if ($mode == "")	{ first(); }
	if ($mode == "del")	{ del(); }
	}

}

//	商品詳細一覧
function first() {
global $main,$goods,$maker,$bunrui,$s_goods,$s_size,$view,$view_s,$view_e,$page,$views,$main_r,$goods_r,$list_num,$mode;
include './array.inc';
include "../../cone.inc";

	//if (!$maker) {							//	del ohkawara 2020/03/30
	if (!$_POST['goods_jan_num'] && !$maker) {	//	add ohkawara 2020/03/30
		echo "負荷軽減の為メーカーを選択してください。<br>\n";
		return ;
	}

	//	del ookawara 2009/08/21
/*
	$sql =	"select distinct d.list_num, d.goods_name, b.maker_name, d.color, d.size, a.price, c.class_l, c.class_m_n" .
			" from goods a, maker b, class c, list d where";
echo $sql."<br>\n";
	if ($maker) { $sql .= " d.maker_num = '$maker'"; }
	if ($maker && $bunrui) { $sql .= " AND"; }
	if ($bunrui) { $sql .= " d.class_m = '$bunrui'"; }
	if ($maker && $bunrui && $s_goods) { $sql .= " AND d.goods_name='$s_goods'"; }
	if ($maker || $bunrui) { $sql .= " AND"; }
	$sql .= " d.class_m = c.class_m AND d.maker_num = b.maker_num AND d.pluid=a.pluid";
	$sql .= " AND d.state!='1'";	//	2009/04/25	add ookawara
	$sql .= " order by d.list_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	$max = $count-1;
	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	if (!$page) { $page = 1; }
	$view_s = $views * ($page-1);
	$view_e = ($views * $page) - 1;
	if ($view_e > $max) { $view_e = $max; }
	if ($count < 1) { echo ("詳細登録されております商品はありません。<BR><BR>\n"); }
*/

	//	add ookawara 2009/08/21
	$count = 0;
	$where = "";
	if ($maker) { $where .= " AND list.maker_num='".$maker."'"; }
	if ($bunrui) { $where .= " AND list.class_m = '".$bunrui."'"; }
	if ($s_goods) { $where .= " AND list.goods_name='".$s_goods."'"; }

	//	add ohkawara 2020/03/30
	if ($_POST['goods_jan_num']) {
		if ($_POST['goods_jan_num'] > "2147483647") {
			 $where .= " AND goods.pluid='".$_POST['goods_jan_num']."'";
		} else {
			$where .= " AND (goods.pluid='".$_POST['goods_jan_num']."' OR list.list_num='".$_POST['goods_jan_num']."')";
		}
	}

	$sql  = "SELECT count(distinct list.list_num) AS count".
			" FROM list list".
			" LEFT JOIN maker maker ON maker.maker_num=list.maker_num".
			" LEFT JOIN goods goods ON goods.pluid=list.pluid".
			" LEFT JOIN class class ON class.class_m=list.class_m".
			" WHERE list.state='0'".
			$where.
			";";
//echo $sql;
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}

	$page_all = 0;
	if ($count > 0) {
		$page = $_POST['page'];
		$views = $VIEW_NUM[$view];
		$page_all = ceil($count / $views);
		if ($page < 1) { $page = 1; }
		if ($page > $page_all) {
			$page = $page_all;
		}
		$max = $count - 1;
		$view_s = $views * ($page - 1);
		$view_e = ($views * $page) - 1;
		if ($view_e > $max) { $view_e = $max; }

		echo <<<EOT
<FORM action='{$PHP_SELF}' method='POST'>
<INPUT type='hidden' name='mode' value='del'>
<INPUT type="hidden" name="main" value="{$main}">
<INPUT type="hidden" name="goods" value="{$goods}">
<INPUT type="hidden" name="maker" value="{$maker}">
<INPUT type="hidden" name="bunrui" value="{$bunrui}">
<INPUT type="hidden" name="s_goods" value="{$s_goods}">
<INPUT type="hidden" name="s_size" value="{$s_size}">
<INPUT type="hidden" name="view" value="{$view}">
<INPUT type="hidden" name="main_r" value="{$main}">
<INPUT type="hidden" name="goods_r" value="{$goods}">
<INPUT type="hidden" name="page" value="{$_POST['page']}">
削除をしたい商品の番号をクリックして下さい。<BR>
Page : {$page}/{$page_all}<BR>
<TABLE border="0" cellpadding="3" cellspacing="2" bgcolor="#999999">
<TBODY>
<TR bgcolor="#cccccc">
<TD align="center">番号</TD>
<TD align="center">名前</TD>
<TD align="center">メーカー名</TD>
<TD align="center">カラー</TD>
<TD align="center">サイズ</TD>
<TD align="center">値段</TD>
<TD align="center">分類</TD>
</TR>
EOT;

		$offset = ($page - 1) * $views;
		$limit = $views;
		$display_start = $offset + 1;
		$display_end = $display_start + $views;
		if ($display_end > $count) { $display_end = $count; }
		$limit_num = " OFFSET ".$offset." LIMIT ".$limit;

		$sql  = "SELECT distinct list.list_num, list.goods_name, maker.maker_name, list.color,".
				" list.size, goods.price, class.class_l, class.class_m_n".
				" FROM list list".
				" LEFT JOIN maker maker ON maker.maker_num=list.maker_num".
				" LEFT JOIN goods goods ON goods.pluid=list.pluid".
				" LEFT JOIN class class ON class.class_m=list.class_m".
				" WHERE state='0'".
				$where.
				" ORDER BY list.list_num".
				$limit_num.";";
		if ($result = pg_query($conn_id,$sql)) {
			while ($list = pg_fetch_array($result)) {
				if (!$list['maker_name']) { $list['maker_name'] = "--"; }
				if (!$list['color']) { $list['color'] = "--"; }
				if (!$list['size']) { $list['size'] = "--"; }
				if (!$list['price']) { $list['price'] = "--"; }
				$cla = $list['class_l'];

				echo "<TR bgcolor='#ffffff'>\n";
				echo "<TD><INPUT type='submit' name='list_num' value='".$list['list_num']."' onclick='return confirm(\"本当に削除してもよろしいでしょうか？\")'></TD>\n";
				echo "<TD>".$list['goods_name']."</TD>\n";
				echo "<TD>".$list['maker_name']."</TD>\n";
				echo "<TD align='center'>".$list['color']."</TD>\n";
				echo "<TD align='center'>".$list['size']."</TD>\n";
				echo "<TD align='right'>".$list['price']." 円</TD>\n";
				echo "<TD>".$CLASS_N[$cla]." : ".$list['class_m_n']."</TD>\n";
				echo "</TR>\n";
			}
		}

		echo <<<EOT
</TBODY>
</TABLE>
</FORM>
EOT;

//ページ処理
		if ($max > $view_e || $view_s >= $views) {
			define("goods_max_num",$max);
			next_p_g_1();
		}

	} else {
		echo "該当する商品はございません。";
	}

	pg_close($conn_id);

}

//	詳細削除記録
function del() {
global $main,$goods,$maker,$bunrui,$view,$view_s,$view_e,$page,$views,$main_r,$goods_r,$list_num;
include './array.inc';
include "../../cone.inc";

	$sql = "select d.goods_name, b.maker_name, d.picture from maker b, list d where d.list_num='$list_num' AND d.maker_num=b.maker_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$lists = pg_fetch_array($sql1,0);
	$goods_name = $lists[0];
	$maker_name = $lists[1];
	$picture    = $lists[2];

//	if ($picture) {
/*	//	del ookawara 2009/08/04
	if ($picture && file_exists("../pic/$picture")) {
		unlink("../pic/$picture");
	}
*/

	//	商品画像ファイルを、フォルダー移動	add ookawara 2009/08/04

	$DIR_PIC = "../pic/";		//	画像フォルダー
	$DIR_MOVE = "../pic_del/";	//	画像移動先フォルダー
	$checkimageName = $list_num.".*";
	$checkFile = $DIR_PIC.$checkimageName;
	$CHECK_IMG = glob($checkFile);
	if ($CHECK_IMG) {
		foreach ($CHECK_IMG AS $key => $val) {
			list($a,$a,$file_name) = explode("/",$val);
			@rename($val,$DIR_MOVE.$file_name);
			@chmod($DIR_MOVE.$file_name, 0666);
		}
	}

//	$sql = "delete from list where list_num='$list_num';";	//	2009/04/25	del ookawara
	//	2009/04/25	add ookawara
	$sql1 = "";
	$sql  = "UPDATE list set".
			" state='1',".
			" update_time=now()".
			" WHERE list_num='".$list_num."';";
	$sql1 = pg_exec($conn_id,$sql);

	if ($sql1) {
		echo ("商品名：$goods_name メーカー：$maker_name を削除しました。<BR><BR>\n");
	}

	pg_close($conn_id);
	first();

}
?>
