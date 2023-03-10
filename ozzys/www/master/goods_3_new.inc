<?PHP
//	商品詳細登録

//	DB設定

	//	テスト設定
	define("SET_TEST","");

	//	ポスデーターテーブル
	define("TABLE_GOODS",SET_TEST."goods");
	//	商品詳細登録テーブル
	define('TABLE_LIST','list');
	//	メーカーテーブル
	define("TABLE_MAKER",SET_TEST."maker");
	//	分類テーブル
	define('TABLE_CLASS','class');



function goods_3() {
include './array.inc';
global $main,$goods,$PHP_SELF,$maker,$bunrui,$view,$check,$view_s,$view_e,$page,$views,$goods_num,$list_num,$pluid,$goods_name,$maker_name,$comment,$size,$color,$picture,$maker_num,$class_m,$display,$files,$files_name,$pic,$regi,$max,$main_r,$goods_r,$mode;
include "../../cone.inc";

	if ($view) {
		while (list($key,$value) = each($_POST)) {
##			if (ereg("[0-9]_x","$key")) {
			if (preg_match("/[0-9]_x/","$key")) {
##				$key = ereg_replace("_x","",$key);
				$key = preg_replace("/_x/","",$key);
				if ($value <= 39) { $value = 2; } else { $value = 1; }
				$dis = "$key:$value";
			}
		}

		if ($mode == "write") {
			first_02();
		} elseif ($mode == "syou" && $dis) {
			change_display($dis);
		} elseif ($mode == "delete") {
			delete_date();
			unset($goods_num);
		}

		if ($mode == "syou" && $goods_num) {
			first_01();
		} else {
			first_00();
		}
	}

}


//	詳細一覧
function first_00() {
include './array.inc';
include "../../cone.inc";
global $PHP_SELF,$main,$goods,$maker,$bunrui,$s_goods,$s_size,$regi,$view,$views,$view_s,$view_e,$page,$max,$stock;

	if (!$maker) {
		echo "負荷軽減の為メーカーを選択してください。<br>\n";
		return ;
	}

	//	分類リスト作成
	$sql  = "SELECT * FROM ".TABLE_CLASS.
			" ORDER BY class_m;";
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$class_l_ = $list[class_l];
			$class_m_ = $list[class_m];
			$class_m_n_ = $list[class_m_n];
			$CLASS_L[$class_m_] = $class_l_;
			$CLASS_NAME[$class_m_] = $class_m_n_;
		}
	}

	//	メーカーリスト作成
	$maker_list = "";
	$count = 0;
	$master_maker_name = "";
	if ($maker) {
		$sql  = "SELECT distinct maker, maker_name FROM ".TABLE_MAKER.
				" WHERE maker_num='".$maker."';";
//	echo "sql<>".$sql."<br>\n";
		if ($result = pg_query($conn_id,$sql)) {
			$count = pg_numrows($result);
			if($count == 1) {
				$list = pg_fetch_array($result);
				$maker_list = " AND goods.maker='".$list['maker']."'";
			} elseif ($count > 1) {
				while ($list = pg_fetch_array($result)) {
					if ($maker_list) { $maker_list .= " OR "; }
					$maker_list .= "goods.maker='".$list['maker']."'";
				}
				if ($maker_list) { $maker_list = " AND (".$maker_list.")"; }
			}
			if ($list['maker_name']) { $master_maker_name = $list['maker_name']; }
		}
	}

	//	該当商品数
	$count = 0;
	$sql  = "SELECT count(distinct goods.goods_num) AS count FROM ".TABLE_GOODS." goods";
	$sql .= " LEFT JOIN ".TABLE_LIST." list ON goods.pluid=list.pluid AND list.state!='1'";
	$sql .= " LEFT JOIN ".TABLE_MAKER." maker ON list.maker_num=maker.maker_num";
	$sql .= " WHERE goods.price>'0'";
	if ($regi == 1) {
		$sql .= " AND list.list_num is NULL";
	} elseif ($regi == 2) {
		$sql .= " AND list.list_num>'0'";
	}
	if ($maker_list) { $sql .= $maker_list; }
	if ($bunrui) { $sql .= " AND goods.class_m='".$bunrui."'"; }
	if ($maker && $bunrui && $s_goods) { $sql .= " AND goods.goods='".$s_goods."'"; }
	if ($s_goods && $s_size) { $sql .= " AND goods.size = '".$s_size."'"; }
	$sql .= ";";
//	echo "sql<>".$sql."<br>\n";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}
	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	if (!$page) { $page = 1; }
	$start = $views * ($page-1);
	$end = $views;
	$limit = " OFFSET ".$start." LIMIT ".$end;
	$max = $count - 1;
	$view_s = $views * ($page-1);
	$view_e = ($views * $page)-1;
	if ($view_e > $max) { $view_e = $max; }
	if ($count < 1) {
		echo "登録されております商品はありません。<BR><BR>\n";
	} else {
		echo <<<EOT
<form action='{$PHP_SELF}' method='POST'>
<input type="hidden" name="mode" value="syou">
<input type="hidden" name="main" value="{$main}">
<input type="hidden" name="goods" value="{$goods}">
<input type="hidden" name="maker" value="{$maker}">
<input type="hidden" name="bunrui" value="{$bunrui}">
<input type="hidden" name="s_goods" value="{$s_goods}">
<input type="hidden" name="s_size" value="{$s_size}">
<input type="hidden" name="regi" value="{$regi}">
<input type="hidden" name="view" value="{$view}">
<input type="hidden" name="main_r" value="{$main}">
<input type="hidden" name="goods_r" value="{$goods}">
<input type="hidden" name="page" value="{$page}">
詳細情報登録・編集したい商品の番号をクリックして下さい。<BR>
該当商品数：{$count}件 Page : {$page}/{$page_all} <BR>
<table border="0" cellpadding="3" cellspacing="2" bgcolor="#999999">
<tr bgcolor="#cccccc">
<th>番号</th>
<th>商品番号</th>
<th>ジャンコード</th>
<th>名前</th>
<th>メーカー名</th>
<th>カラー</th>
<th>サイズ</th>
<th>値段</th>
<th>分類</th>
<th>在庫数</th>
<th>表示切替</th>
</tr>

EOT;

		$sql  = "SELECT distinct goods.goods_num, goods.pluid, goods.goods, goods.maker, goods.color,";
		$sql .= " goods.size, goods.price, goods.class_m, goods.stock,";
		$sql .= " list.list_num, list.pluid AS l_pluid, list.goods_name, list.size AS l_size, list.color AS l_color,";
		$sql .= " list.maker_num, list.class_m AS l_class_m, list.display,";
		$sql .= " maker.maker_name";
		$sql .= " FROM ".TABLE_GOODS." goods";
		$sql .= " LEFT JOIN ".TABLE_LIST." list ON goods.pluid=list.pluid AND list.state!='1'";
		$sql .= " LEFT JOIN ".TABLE_MAKER." maker ON list.maker_num=maker.maker_num";
		$sql .= " WHERE goods.price>'0'";
		if ($regi == 1) {
			$sql .= " AND list.list_num is NULL";
		} elseif ($regi == 2) {
			$sql .= " AND list.list_num>'0'";
		}
		if ($maker_list) { $sql .= $maker_list; }
		if ($bunrui) { $sql .= " AND goods.class_m='".$bunrui."'"; }
		if ($maker && $bunrui && $s_goods) { $sql .= " AND goods.goods='".$s_goods."'"; }
		if ($s_goods && $s_size) { $sql .= " AND goods.size = '".$s_size."'"; }
		$sql .= " ORDER BY goods.goods_num";
		$sql .= $limit.";";
//echo "sql<>".$sql."<br>\n";
		if ($result = pg_query($conn_id,$sql)) {
			while ($list = pg_fetch_array($result)) {
				$goods_num = $list['goods_num'];
				if ($list['list_num']) {
					$list_num = $list['list_num'];
				} else {
					$list_num = "---";
				}
				if ($list['l_pluid']) {
					$pluid = $list['l_pluid'];
				} else {
					$pluid = $list['pluid'];
				}
				if ($list['goods_name']) {
					$goods_name = $list['goods_name'];
				} else {
					$goods_name = $list['goods'];
				}
##				$goods_name = ereg_replace("#","＃",$goods_name);
				$goods_name = preg_replace("/#/","＃",$goods_name);
				if ($master_maker_name) {
					$maker_name = $master_maker_name;
				} elseif ($list['maker_name']) {
					$maker_name = $list['maker_name'];
				} elseif ($list['maker_name']) {
					$maker_name = $list['maker'];
				}
				if ($list['l_color']) {
					$goods_color = $list['l_color'];
				} elseif ($list['color']) {
					$goods_color = $list['color'];
				} else {
					$goods_color = "---";
				}
				if ($list['l_size']) {
					$goods_size = $list['l_size'];
				} elseif ($list['size']) {
					$goods_size = $list['size'];
				} else {
					$goods_size = "---";
				}
				$price = $list['price'];
				$class_name = "";
				if ($list['l_class_m']) {
					$class_m = $list['l_class_m'];
				} else {
					$class_m = $list['class_m'];
				}
				if ($class_m) {
					$class_l = $CLASS_L[$class_m];
					$class_name = $CLASS_N[$class_l]."：".$CLASS_NAME[$class_m];
				}
				$stock = $list['stock'];
				$display = $list['display'];
				if ($display == 1) {
					$goods_display = "非表示";
					$goods_display = "<INPUT type='image' name='".$list_num.":2' value='".$display."' src='./images/hihyouji.gif'>";
				} elseif ($display == 2) {
					$goods_display = "表示";
					$goods_display = "<INPUT type='image' name='".$list_num.":1' value='".$display."' src='./images/hyouji.gif'>";
				} else {
					$goods_display = "--";
				}

				echo "<tr bgcolor='#ffffff'>\n";
				echo "<td><input type='submit' name='goods_num' value='".$goods_num."'></td>\n";
				echo "<td align='center'>".$list_num."</td>\n";
				echo "<td>".$pluid."</td>\n";
				echo "<td>".$goods_name."</td>\n";
				echo "<td>".$maker_name."</td>\n";
				echo "<td align='center'>".$goods_color."</td>\n";
				echo "<td align='center'>".$goods_size."</td>\n";
				echo "<td align='right'>".$price."円</td>\n";
				echo "<td>".$class_name."</td>\n";
				echo "<td align=\"center\">".$stock."</td>\n";
				echo "<td align=\"center\">".$goods_display."</td>\n";
				echo "</tr>\n";
			}
		}

		echo <<<EOT
</table>
</form>

EOT;

		//ページ処理
		if ($max > $view_e || $view_s >= $views) { next_p_g_1(); }

	}

	pg_close($conn_id);

}

//	詳細登録画面
function first_01() {
include './array.inc';
include "../../cone.inc";
global $PHP_SELF,$mode,$main,$goods,$maker,$bunrui,$regi,$view,$main_r,$goods_r,$page,$goods_num,$s_goods,$s_size;

	if (!$_POST['goods_num']) {
		$ERROR[] = "登録編集する商品の番号が選択されておりません。";
		ERROR(&$ERROR);
	}

	//	pluid抜き出し
	$pluid = "";
	$sql  = "SELECT pluid FROM goods".
			" WHERE goods_num='".$_POST['goods_num']."'".
			" LIMIT 1;";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$pluid = $list['pluid'];
	}

	//	詳細データーあるかチェック
	$list_num = "";
	$sql  = "SELECT * FROM list".
			" WHERE pluid='".$pluid."'".
			" ORDER BY list_num".
			" LIMIT 1;";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$list_num	= $list['list_num'];
//		$pluid		= $list['pluid'];
		$goods_name	= $list['goods_name'];
##		$goods_name	= ereg_replace("#","＃",$goods_name);
		$goods_name	= preg_replace("/#/","＃",$goods_name);
		$comment	= $list['comment'];
		$size		= $list['size'];
		$color		= $list['color'];
		$picture	= $list['picture'];
		$maker_num	= $list['maker_num'];
		$class_m	= $list['class_m'];
		$display	= $list['display'];
		$c_link		= $list['c_link'];
	}

	//	詳細データーが無ければポスデーターから取得
	if (!$list_num) {
		$sql  = "SELECT class_m, maker, goods, color, size FROM goods".
				" WHERE pluid='".$pluid."'".
				" ORDER BY goods_num".
				" LIMIT 1;";
		if ($result = pg_query($conn_id,$sql)) {
			$list = pg_fetch_array($result);
			$class_m = $list['class_m'];
			$maker_name = $list['maker'];
			$goods_name = $list['goods'];
##			$goods_name	= ereg_replace("#","＃",$goods_name);
			$goods_name	= preg_replace("/#/","＃",$goods_name);
			$color = $list['color'];
			$size = $list['size'];
		}
	}

	//	メーカー番号取得
	if (!$maker_num) {
		$sql  = "SELECT maker_num FROM maker".
				" WHERE maker='".$maker_name."'".
				" LIMIT 1;";
		if ($result = pg_query($conn_id,$sql)) {
			$list = pg_fetch_array($result);
			$maker_num = $list['maker_num'];
		}
	}

	//	メーカーリスト作成
	$maker_list = "";
	$sql  = "SELECT distinct maker_num, maker_name FROM maker".
			" ORDER BY maker_name;";
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$selected = "";
			if ($maker_num == $list['maker_num']) { $selected = " selected"; }
			$maker_list .= "<option value='".$list['maker_num']."'".$selected.">".$list['maker_name']."</option>\n";
		}
	}

/*
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	if ($count > 0) {
		$id = pg_fetch_array($sql1,0);
		$list_num	= $id[0];
//		$pluid		= $id[1];
		$goods_name	= $id[2];
		$goods_name	= ereg_replace("#","＃",$goods_name);
		$comment	= $id[3];
		$size		= $id[4];
		$color		= $id[5];
		$picture	= $id[6];
		$maker_num	= $id[7];
		$class_m	= $id[8];
		$display	= $id[9];
		$c_link		= $id[10];

		if ($maker_num) {
			$sql = "select maker_name from maker where maker_num='$maker_num' order by maker_num;";
			$sql1 = pg_exec($conn_id,$sql);
			$id = pg_fetch_array($sql1,0);
			$maker_name = $id[0];
		}

		if (!$maker_name && $maker_id) {
			$sql = "select maker_name from maker where maker_id='$maker_id' order by maker_id;";
			$sql1 = pg_exec($conn_id,$sql);
			$count = pg_numrows($sql1);
			if ($count > 0) {
				$id = pg_fetch_array($sql1,0);
				$maker_name = $id[0];
				}
		}
	} else {	//	無ければ商品データーから抜き出し
		$sql = "select goods_name, maker_id, class_m from goods where pluid='$pluid' order by goods_num;";
		$sql1 = pg_exec($conn_id,$sql);
		$id = pg_fetch_array($sql1,0);
		$maker_id = $id[1];
		$class_m = $id[2];
		$goods_names = split(":","$id[0]");
//		if (!$maker_name) { $maker_name = $goods_names[0]; }
		if (!$goods_name) {
			$goods_name = $goods_names[1];
			$goods_name = ereg_replace("#","＃",$goods_name);
		}
		if (!$color) { $color = $goods_names[2]; }
		if (!$size)  { $size  = $goods_names[3]; }

		if (!$maker_name && $maker_id) {
			$sql = "select maker_name, maker_num from maker where maker_id='$maker_id' order by maker_id;";
			$sql1 = pg_exec($conn_id,$sql);
			$count = pg_numrows($sql1);
			if ($count >= 1) {
				$id = pg_fetch_array($sql1,0);
				$maker_name = $id[0];
				$maker_num = $id[1];
			}
		}
	}
*/

//	if ($class_m) {
//		$sql = "select class_m_n from class where class_m='$class_m' order by class_m;";
//		$sql1 = pg_exec($conn_id,$sql);
//		$id = pg_fetch_array($sql1,0);
//		$class_m_n = $id[0];
//	}

	if ($picture) { $pic_file = "../pic/$picture"; }
	$dis1 = "";
	$dis2 = "";
	if ($display == 0 || $display == 1)  { $dis2 = " checked"; } else { $dis1 = " checked"; }


	echo <<<EOT
	<FORM action='{$PHP_SELF}' method="POST" enctype='multipart/form-data'>
	<INPUT type="hidden" name="mode" value="write">
	<INPUT type="hidden" name="main" value="{$main}">
	<INPUT type="hidden" name="goods" value="{$goods}">
	<INPUT type='hidden' name='maker' value='{$maker}'>
	<INPUT type='hidden' name='bunrui' value='{$bunrui}'>
	<INPUT type="hidden" name="s_goods" value="{$s_goods}">
	<INPUT type="hidden" name="s_size" value="{$s_size}">
	<INPUT type='hidden' name='view' value='{$view}'>
	<INPUT type='hidden' name='list_num' value='{$list_num}'>
	<INPUT type='hidden' name='pluid' value='{$pluid}'>
	<INPUT type='hidden' name='maker_name' value='{$maker_name}'>
	<INPUT type='hidden' name='maker_num' value='{$maker_num}'>
	<INPUT type='hidden' name='picture' value='{$picture}'>
	<INPUT type="hidden" name="regi" value="{$regi}">
	<INPUT type="hidden" name="main_r" value="{$main}">
	<INPUT type="hidden" name="goods_r" value="{$goods}">
	<INPUT type="hidden" name="page" value="{$page}">

EOT;
	if ($list_num) {
		echo "商品番号：".$list_num." <!--分類：".$class_m_n."--><BR>";
	}

 	echo <<<EOT
     <TABLE border='0' cellpadding='3' cellspacing='2' bgcolor='#999999'>
        <TBODY>
          <TR bgcolor='#cccccc'>
            <TD>商品名</TD>
            <TD><INPUT size='50' type='text' name='goods_name' value='{$goods_name}'></TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>メーカー</TD>
            <TD>
<select name="maker_num">
{$maker_list}
</select>
            </TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>画　像</TD>
            <TD>

EOT;

		if (file_exists($pic_file)) {
			echo <<<EOT
            <P align='center'><IMG src='{$pic_file}' border='0'></P>
            <INPUT type='checkbox' name='pic' value='del'>画像削除する。<BR>

EOT;
		}

		echo <<<EOT
            画像サイズ：250pic x 100pic 以下<BR>
            <INPUT size='36' type='file' name='files'></TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>コメント</TD>
            <TD><TEXTAREA rows='5' cols='55' name='comment'>{$comment}</TEXTAREA></TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>カラー</TD>
            <TD><INPUT size='20' type='text' name='color' value='{$color}'></TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>サイズ</TD>
            <TD><INPUT size='20' type='text' name='size' value='{$size}'></TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>分類</TD>
            <TD><select name="class_m">

EOT;

		$sql  = "SELECT * FROM class ORDER BY class_m;";
		if ($result = pg_query($conn_id,$sql)) {
			while ($list = pg_fetch_array($result)) {
				$class_m_ = $list[class_m];
				$class_m_n_ = $list[class_m_n];

				if ($class_m == $class_m_) { $selected = "selected"; } else { $selected = ""; }
				echo("<option value=\"$class_m_\" $selected>$class_m_n_($class_m_)</option>\n");
			}
		}

		echo <<<EOT
            </select>
            </TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>商品一覧表示</TD>
            <TD><INPUT type="radio" name="display" value="2" {$dis1}>する
				<INPUT type="radio" name="display" value="1" {$dis2}>しない</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>カラー画像LINK</TD>
            <TD><INPUT size='55' type='text' name='c_link' value='{$c_link}'></TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD colspan='2'><INPUT type='submit' value='登録'> <input type="reset" value="リセット"></TD>
          </TR>
        </TBODY>
      </TABLE>
      </FORM>
		<FORM action="{$PHP_SELF}" method="POST">
			<INPUT type="hidden" name="main" value="{$main}">
			<INPUT type="hidden" name="goods" value="{$goods}">
			<INPUT type="hidden" name="maker" value="{$maker}">
			<INPUT type="hidden" name="bunrui" value="{$bunrui}">
			<INPUT type="hidden" name="s_goods" value="{$s_goods}">
			<INPUT type="hidden" name="s_size" value="{$s_size}">
			<INPUT type="hidden" name="display" value="{$display}">
			<INPUT type="hidden" name="regi" value="{$regi}">
			<INPUT type="hidden" name="view" value="{$view}">
			<INPUT type="hidden" name="main_r" value="{$main}">
			<INPUT type="hidden" name="goods_r" value="{$goods}">
			<INPUT type="hidden" name="page" value="{$page}">
			<INPUT type="submit" value="戻る">
		</FORM>

EOT;


	pg_close($conn_id);

}


//	記録更新
function first_02() {
include './array.inc';
include "../../cone.inc";
global $PHP_SELF,$list_num,$pluid,$goods_name,$pic,$picture,$files,$comment,$color,$size,$display,$c_link,$picture,$maker_num,$class_m,$display,$files_name,$maker_name;

//	登録番号チェック
	if (!$list_num) {
		$sql = "select * from list order by list_num;";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);
		if ($count < 1) {
			$list_num = 1;
		} else { 
			$max = $count - 1;
			$id = pg_fetch_array($sql1,$max);
			$list_num = $id[0] + 1;
		}

		if ($files == "aaa") {
			unlink("../pic/$picture"); $picture = "";
		} elseif ($files != "none") {
##			$kaku = split("\.",$files_name);
			$kaku = explode("\.",$files_name);
			$picture = "$list_num.$kaku[1]";
			copy($files,"../pic/$picture");
		}

		$sql = "select pluid from list where pluid='$pluid'order by list_num;";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);

		if (!$count) {
##			$goods_name = ereg_replace("#","＃",$goods_name);
			$goods_name = preg_replace("/#/","＃",$goods_name);
			$sql = "insert into list values (" .
					"'$list_num'," .
					"'$pluid'," .
					"'$goods_name'," .
					"'$comment'," .
					"'$size'," .
					"'$color'," .
					"'$picture'," .
					"'$maker_num'," . 
					"'$class_m'," .
					"'$display'," .
					"'$c_link',".
					"'0');";
			$sql1 = pg_exec($conn_id,$sql);

##			$goods_name = split(":","$goods_name");
			$goods_name = explode(":","$goods_name");

			if ($sql1) {
				echo ("	<TABLE border='0' cellpadding='3' cellspacing='2' bgcolor='#999999'>\n");
				echo ("	  <TBODY>\n");
				echo ("<TR bgcolor='#ffffff'><TD>メーカー名</TD><TD>$maker_name </TD></TR>\n");
				echo ("<TR bgcolor='#ffffff'><TD>商品番号</TD><TD>$list_num </TD></TR>\n");
				echo ("<TR bgcolor='#ffffff'><TD>商品名</TD><TD>$goods_name[0] </TD></TR>\n");
				echo ("	  </TBODY>\n");
				echo ("	</TABLE>\n");
				echo ("<BR>\nを登録いたしました。<BR><BR>\n");
			}
		}
	} else {

		if ($pic == "del") {
			if (file_exists("../pic/$picture")) {
				unlink("../pic/$picture");
			}
			$picture = "";
		}
		if ($files && ($files != "none")) {
##			$kaku = split("\.",$files_name);
			$kaku = explode("\.",$files_name);
			$picture = "$list_num.$kaku[1]";
			copy($files,"../pic/$picture");
		}

		$sql = "update list set" .
				" goods_name='$goods_name'," .
				" comment='$comment'," .
				" size='$size'," .
				" color='$color'," .
				" picture='$picture'," .
				" class_m='$class_m'," .
				" maker_num='$maker_num'," .
				" display='$display'," .
				" c_link='$c_link'" .
				" where list_num=$list_num;";
		$sql1 = pg_exec($conn_id,$sql);

##		$goods_name = split(":","$goods_name");
		$goods_name = explode(":","$goods_name");

		if ($sql1) {
				echo ("	<TABLE border='0' cellpadding='3' cellspacing='2' bgcolor='#999999'>\n");
				echo ("	  <TBODY>\n");
				echo ("<TR bgcolor='#ffffff'><TD>メーカー名</TD><TD>$maker_name </TD></TR>\n");
				echo ("<TR bgcolor='#ffffff'><TD>商品番号</TD><TD>$list_num </TD></TR>\n");
				echo ("<TR bgcolor='#ffffff'><TD>商品名</TD><TD>$goods_name[0] </TD></TR>\n");
				echo ("	  </TBODY>\n");
				echo ("	</TABLE>\n");
				echo ("<BR>\nを更新いたしました。<BR><BR>\n");
		}

	}

//	first_00();	//	2009/02/08 ookawara del

	pg_close($conn_id);

}


//	商品削除処理
function delete_date() {

	//	state = 1 にする。
	//	画像を削除（別フォルダーに移動）





}


//	表示切り替え
function change_display($dis) {
include './array.inc';
include "../../cone.inc";

##	$disp = split("\:",$dis);
	$disp = explode("\:",$dis);
	$sql = "update list set display='$disp[1]' where list_num='$disp[0]';";
	$sql1 = pg_exec($conn_id,$sql);
}
?>
