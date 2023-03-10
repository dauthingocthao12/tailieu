<?PHP
//	入荷情報
function news_display() {
global $db,$LOGDATA_DIR,$news_file,$index,$h_tax,$tax;

	$html = "";

	$file = "$LOGDATA_DIR/$news_file";
	if (!$LIST && file_exists($file)) {
		$LIST = file($file);
	}

	$goods = "";
	unset($LIST[0]);
	$max = count($LIST);
	if ($max > 0) {
		$i = 0;
		foreach ($LIST AS $val) {
##			$val = eregi_replace("[^0-9]","",$val);
			$val = preg_replace("/[^0-9]/i","",$val);
			$val = trim($val);
			if (!$val) { continue; }

			$amari = $i % 2;

			if ($amari != 1) {
				$goods_list .= "<div class='products-line'>\n";
			}

			$sql  = "SELECT list.list_num, list.goods_name, list.size, list.color,".
				" list.picture, maker.maker_name, goods.price".
				" FROM list".
				" LEFT JOIN maker on list.maker_num=maker.maker_num".
				" LEFT JOIN goods on list.pluid=goods.pluid".
				" WHERE list.list_num='".$val."'".
				" AND list.state!='1'".		//	2009/04/25	add ookawara
				" LIMIT 1;";
			if ($result = pg_query($db,$sql)) {
				$list = pg_fetch_array($result);

				$name = $list[maker_name]."・".$list[goods_name];
				$url = "/goods/n".$list['list_num'].'/';
				$img_url = "/pic/".$list[picture];
				$size = $list[size];
				if (!$size) { $size = "--"; }
				$color = $list[color];
				if (!$color) { $color = "--"; }

				if ($list[picture] && file_exists(".$img_url")) {
					$set_width = 125;
					$plist = @getimagesize(".$img_url");
					$wid = $plist[0];
					$hig = $plist[1];
					if ($wid > $set_width) {
						$ritu = $set_width / $wid;
						$width = $wid * $ritu;
						$height = $hig * $ritu;
					} else {
						$width = $wid;
						$height = $hig;
					}
					// picture
					$img = "<img src=\"$img_url\" width=\"{$width}\" height=\"{$height}\" border=\"0\" alt=\"No.{$list['list_num']} {$goods_name}\" class=\"item-picture\">\n";
				} else {
					$img = "";
				}
				$price = $list[price];
				if ($h_tax == 1) {
					$price = $price + floor(($price * $tax) + 0.5);
				}
				$price = number_format($price);

				$goods_list .= <<<EOT
<div class="product-item">
	<div class="item-title"><A href="{$url}">{$name}</A></div>
	<div class="cols2 col-pic">
		<A href="{$url}">{$img}</A>
	</div>
	<div class="cols2 col-dtls">
		サイズ：{$size}<BR>
		カラー：{$color}<BR>
		値段：<B><FONT color="#ff0000">{$price}円</FONT></B>
	</div>
</div>

EOT;

				if ($amari == 1) {
					$goods_list .= "</div>\n";
				}

				$i++;
			}
		}

		if ($amari != 1) {
			$goods_list .= "</div>\n";
		}
	}

	if ($goods_list) {
		$html = <<<EOT
<div id="products-top" class="products-table products-list">
$goods_list
</div>
EOT;
	} else {
//		$html = "test";
	}

	return $html;

}
?>
