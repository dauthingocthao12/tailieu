<?php
class Breadcrumb{

	private static $parts = array(); // 例: array('text' => 'TOP', 'link' => '/')

	/**
	 * breadcrumbsのパーツ追加
	 *
	 * @author Azet
	 * @param string $txt
	 * @param string $link
	 */
	static function add($txt, $link_) {
		$link = trim($link_, '/'); // remove / at head and tail
		if($link=='') {
			$link = "rank"; // default mode
		}
		self::$parts[] = array(
			'text' => $txt,
			'link' => smarty_function_sitelink( array('mode' => $link) )
		);
	}

	/**
	 * breadcrumbsのパーツ全て削除
	 *
	 */
	static function deleteAll(){
		self::$parts = array();
	}

	static function Date_Convert($date){
		// 例）20170314 => 2017年3月14日
		$dt=date('Y年n月j日', strtotime($date));
		return $dt;
	}

	static function Star_Convert($star,$en_star){
		// 例）aquarius => みずがめ座
		$s=array_search ($star,$en_star); 
		return $s;
	}

	/**
	 * HTMLパンくずリスト作成j
	 *
	 * @author Azet
	 * @return string HTML
	 */
	static function getAdd(){
		// output
		$breadlist = "";

		$length = count(self::$parts);
		$no = 1; 
		//$breadlist="<li><a href='/'> TOP</a></li>";
		$makeli="";
		foreach(self::$parts as $value){
			$icon = "";
			if($no==1) {
				$icon = "<span class='glyphicon glyphicon-home' aria-hidden='true'></span> ";
			}
			if($no == $length){
				if($value['text'] == 'TOP'){
					$makeli="<li class='active' itemprop='itemListElement' itemscope itemtype='https://schema.org/ListItem'>$icon<span itemprop='name' title='星座占い'>{$value['text']}</span><meta itemprop='position' content='$no'/></li>\n";
				}else{
					$makeli="<li class='active' itemprop='itemListElement' itemscope itemtype='https://schema.org/ListItem'>$icon<span itemprop='name'>{$value['text']}</span><meta itemprop='position' content='$no'/></li>\n";
				}
			}else{
				if($value['text'] == 'TOP'){
					$makeli="<li itemprop='itemListElement' itemscope itemtype='https://schema.org/ListItem'><a itemprop='item' href='{$value['link']}' title='12星座占い'>$icon<span itemprop='name'>{$value['text']}</span></a><meta itemprop='position' content='$no'/></li>\n";
				}else{
					$makeli="<li itemprop='itemListElement' itemscope itemtype='https://schema.org/ListItem'><a itemprop='item' href='{$value['link']}'>$icon<span itemprop='name'>{$value['text']}</span></a><meta itemprop='position' content='$no'/></li>\n";
				}
			}
			$no++;
			$breadlist .= $makeli;
		}

		return $breadlist;
	}

}
?>
