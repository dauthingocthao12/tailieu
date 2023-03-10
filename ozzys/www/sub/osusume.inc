<?PHP
//	おすすめ
function osusume($LIST, $responsive_=false) {
global $db,$LOGDATA_DIR,$oshirase_file,$osusume_file,$index;

	//	おすすめ画像サイズ
	$xpic = 170;


	//	お知らせ
	$file = "$LOGDATA_DIR/$oshirase_file";
	if (file_exists($file)) {
		$oshirase_html = file_get_contents($file);
		$oshirase_html = trim($oshirase_html);
	}

	if ($oshirase_html) {
		if($responsive_) {
			$oshirase_html = <<<WAKABA
			<section>
				<h2 class="sub-title-hours">★お知らせ</h2>
				<div class="product-updates">
					$oshirase_html
				</div>
			</section>

WAKABA;
		}
		else {
			$oshirase_html = <<<WAKABA
			<TABLE border="0" width="195" bgcolor="#ff0000" height="25">
				<TBODY>
					<TH><FONT color="#ffffff">★お知らせ</FONT></TH>
				</TBODY>
			</TABLE>
			<BR>
			<TABLE border="0" width="170" bgcolor="#000000" cellspacing="1" cellpadding="0">
				<TR bgcolor="#ffffff">
					<TD style="line-height : 15px;padding : 3px;">
					$oshirase_html
					</TD>
				</TR>
			</TABLE>
WAKABA;
		}
	}

	$file = "$LOGDATA_DIR/$osusume_file";
	if (!$LIST && file_exists($file)) {
		$LIST = file($file);
	}

	$osusume = <<<WAKABA
	<h2 class="sub-title-hours">★お勧め商品</h2>

WAKABA;

	$goods = "";
	if ($LIST) {
		foreach ($LIST AS $VAL) {
			$VAL = trim($VAL);
##			$val = eregi_replace("[^0-9]","",$VAL);
			$val = preg_replace("/[^0-9]/i","",$VAL);
			if ($val) {
				$sql  = "SELECT goods_name, picture, comment FROM list" .
						" WHERE list_num='".$val."'".
						" AND state!='1'".		//	2009/04/25	add ookawara
						" LIMIT 1;";
				if ($result = pg_query($db,$sql)) {
					$list = pg_fetch_array($result);
					$goods_name = $list[goods_name];
					$picture = $list[picture];
					$pic_url = "/pic/$picture";
					if ($picture && file_exists(".$pic_url")) {
						$plist = getimagesize(".$pic_url");
						$wid = $plist[0];
						$hig = $plist[1];
						if ($wid > $gpx) {
							$ritu = $xpic / $wid;
							$width_ = $wid * $ritu;
							$height_ = $hig * $ritu;
						}
						else {
							$width_ = $wid;
							$height_ = $hig;
						}
						$img_msg = "<IMG class=\"img-responsive img-center\" src=\"$pic_url\" width=\"$width_\" height=\"$height_\" border=\"0\" alt=\"$goods_name\">\n";
					} else { $img_msg = ""; }

					$comment = $list[comment];
					$comment = nl2br($comment);

					if ($goods_name == "") { continue; }	//	add ookawara 2013/06/20

					$goods .= <<<WAKABA
					<TABLE class="table-resp table-resp-nogap product-updates">
						<TBODY>
						<TR>
							<TH height="20"><A href="/goods/$VAL/$index">$goods_name</A></TH>
						</TR>
						<TR>
							<TD align="center"><A href="/goods/$VAL/$index">$img_msg</A></TD>
						</TR>
						<TR>
							<TD style="line-height : 15px;padding : 3px;">
							$comment
							</TD>
						</TR>
						</TBODY>
					</TABLE>

WAKABA;
				}
			}
		}
	}

	if ($goods) {
		$osusume .= $goods;
	}
	else {
		$osusume .= "今現在おすすめ情報は登録されておりません。<BR>\n<BR>\n";
		unset($osusume);
	}

	$html  = $oshirase_html;
	$html .= $osusume;


	return $html;

}
?>
