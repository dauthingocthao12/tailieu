<?PHP
//	顧客情報管理	************************************************************************
//	顧客ポイント情報
function kokyaku_4() {
global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$kojin_num;

	if ($view) {
		first_00();
	}

}



function first_00() {
global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$max,$view_s,$views,$view_e;
include './array.inc';
include "../../cone.inc";

	## $num_k_ = mb_convert_kana($num_k_,"n","EUC-JP");
	$num_k_ = mb_convert_kana($num_k_,"n","UTF-8");

	$count = 0;
	$where = "";
	if ($num_k_) {
		$where .= " AND e.kojin_num='$num_k_'";
	} elseif ($prf_ || $name_s_) {
		if ($prf_) {
			$where .= " AND e.prf='$prf_'";
		}
		if ($name_s_) {
			$where .= " AND e.name_s='$name_s_'";
		}
	}
	$sql  = "SELECT count(distinct e.kojin_num) AS count FROM kojin e".
			" WHERE e.point!='0'";
	$sql .= " AND e.saku='0'";
	$sql .= $where.";";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}

	if ($count < 1) {
		echo ("ポイント所持のお客様はいません。<BR><BR>\n");
	} else {
		$views = $VIEW_NUM[$view];
		$page_all = ceil($count / $views);
		if (!$page) {
			$page = 1;
		} elseif ($page > $page_all) {
			$page = $page_all;
		}
		$offset = ($page - 1) * $views;
		$limit = $views;
		$display_start = $offset + 1;
		$display_end = $display_start + $views - 1;
		if ($display_end > $count) { $display_end = $count; }
		//	検索数
		$limit_num = " OFFSET {$offset} LIMIT {$limit}";

		//	旧システムに対応のため
		$max = $count - 1;
		$view_s = $views * ($page-1);
		$view_e = ($views * $page) - 1;
		if ($view_e > $max) { $view_e = $max; }

		echo <<<EOT
詳細を見たいお客様の番号を押して下さい。 <a href="./make_point_csv.php" target="_blank">csvダウンロード</a><BR>
ご購入お客様{$count}人中 {$display_start} - {$display_end}人目 ({$page}/{$page_all} Page)<br>
<TABLE border="0" bgcolor="#999999">
    <TR bgcolor="#cccccc" align='center'>
      <TD>番号</TD>
      <TD>名前</TD>
      <TD>住所</TD>
      <TD>メールアドレス</TD>
      <TD>ポイント</TD>
    </TR>

EOT;

		$sql  = "SELECT distinct e.kojin_num, e.name_s, e.name_n,".
				" e.prf, e.city, e.add1, e.add2, e.email, e.point".
				" FROM kojin e".
				" WHERE e.point!='0'";
		$sql .= " AND e.saku='0'";
		$sql .= $where." ORDER BY e.kojin_num".$limit_num.";";
		if ($result = pg_query($conn_id,$sql)) {
			WHILE ($list = pg_fetch_array($result)) {
				foreach($list AS $key => $val) {
					$$key = trim($val);
				}
				echo <<<OZZYS
    <TR bgcolor="#ffffff" align='center'>
      <TD>
       $kojin_num
      </TD>
      <TD>
       $name_s $name_n
      </TD>
      <TD align='left'>
       $PRF_N[$prf] $city $add1 $add2
      </TD>
      <TD align='left'>
       <A href="mailto:$email">$email</A>
      </TD>
      <TD>
       $point
      </TD>
    </TR>

OZZYS;
			}
		}


		echo <<<EOT
</TABLE>
<br>

EOT;

		if ($max > $view_e || $view_s >= $views) { next_p_k(); }

	}

	pg_close($conn_id);

}
?>
