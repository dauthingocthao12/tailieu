<?php

class OzzysSEO {

	const DESCRIPTION_MAX_LENGTH = 120;
	const KEYWORDS_TAIL          = "オジーズ,Ozzys";


	static private $keywords = array(
		 'general' => array()
	 	,'breadcrumb' => array()
	);


	static private $descriptions = array(
		 'general' => array()
	);


	static private $product = null;


	// === Keywords ===

	/**
	 * キーワードをSEO用のリストに追加
	 *
	 * @author Azet
	 * @param string $keyword_
	 */
	static function addKeyword($keyword_) {
		self::$keywords['general'][] = $keyword_;
	}


	/**
	 * パンくずリストの部分をSEO用のリストに追加
	 * 自動description勝利の時に使います
	 *
	 * @author Azet
	 * @param string $keyword_
	 */
	static function addKeywordBreadcrumb($keyword_) {
		self::$keywords['breadcrumb'][] = $keyword_;
	}


	/**
	 * キーワードのメタタグ作成
	 *
	 * @author Azet
	 * @return string HTML
	 */
	static function makeMetaKeywords() {
		$html = "<meta name=\"keywords\" content=\"";
		$html .= join(self::decideKeywords(), ',').','.self::KEYWORDS_TAIL;
		$html .= "\" />";

		$html = self::stripKeywords($html);

		return $html;
	}


	/**
	 * キーワードリストを準備する機能
	 *
	 * @author Azet
	 * @return Array
	 */
	static private function decideKeywords() {
		$keywords = array();

		$keywords = self::pickKeywords(self::$keywords['general']);

		return $keywords;
	}


	/**
	 * キーワードを選ぶ機能
	 *
	 * @author Azet
	 * @param Array $keywords_
	 * @return Array
	 */
	static private function pickKeywords($keywords_) {
		$keywords = array();

		if( count($keywords_)>0 ) {

			$kw = $keywords_;

			$loop = 0;
			$added_entry = 0;
			while($added_entry<5 && isset($kw[$loop]) ) {
				$keywords[] = $kw[$loop];
				$added_entry += 1;
				$loop += 1;
			}
		}

		return $keywords;
	}


	// === Description ===


	/**
	 * descriptionのリストに追加
	 *
	 * @author Azet
	 * @param string $description_
	 */
	static function addDescription($description_) {
		self::$descriptions['general'][] = $description_;
	}


	/**
	 * descriptionのメタタグを作成
	 *
	 * @author Azet
	 * @return string HTML
	 */
	static function makeMetaDescription() {
		$html = "<meta name=\"description\" content=\"";
		$html .= self::decideDescription();
		$html .= "\" />";

		$html = self::stripDescription($html);

		return $html;
	}


	/**
	 * descriptionを準備する機能
	 *
	 * @author Azet
	 * @return string
	 */
	static private function decideDescription() {
		$description = "";

		if( isset(self::$product) ) {

			// triming function test
			//self::$product['comment'] = "ああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああ";

			//print_r(self::$product);

			if(self::$product['comment'] != "　") {
				// product comment as description
				$description .= self::$product['comment'];
			}
			else {
				// dynamic description
				$description .= self::$product['name']."の商品情報です。";
				$description .= "メーカー: "  . self::$product['maker'].",";
				$description .= "サイズ: "    . self::$product['size'].",";
				$description .= "カラー: "    . self::$product['color'].",";
				$description .= "JANコード: " . self::$product['jancode'];
			}
		}
		else if( isset(self::$descriptions['general'][0]) ) {
			$description .= self::$descriptions['general'][0];
		}
		else {
			// autogeneration from keywords
			$description .= join(self::$keywords['breadcrumb'], 'の').'です。';
		}

		// lastly, if too long, we trim it
		$description = self::trim($description, self::DESCRIPTION_MAX_LENGTH);

		return $description;
	}


	// === products ===


	/**
	 * 商品の情報設定
	 *
	 * @author Azet
	 * @param array $details_
	 */
	static function setProductDetails($details_) {
		self::$product = $details_;
	}
	

	// === Utilities ===


	/**
	 * キーワードの内奥を整理する機能
	 *
	 * @author Azet
	 * @param string $data_
	 * @return string
	 */
	private static function stripKeywords($data_) {
		$data = $data_;

		$data = preg_replace("/\(No.\d+?\)/", "", $data);
		$data = str_replace(" 一覧", "", $data);

		return $data;
	}


	/**
	 * descriptionの内奥を整理する機能
	 *
	 * @author Azet
	 * @param string $data_
	 * @return string
	 */
	static function stripDescription($data_) {
		$data = $data_;

		$data = preg_replace("/\(No.\d+?\)/", "", $data);

		return $data;
	}


	/**
	 * trim too long data if needed
	 * multibyte support
	 *
	 * @author Azet
	 * @param string $data_ data to trim
	 * @param int $max_length_ max data length
	 * @return string
	 */
	static function trim($data_, $max_length_) {
		if(mb_strlen($data_) > $max_length_) {
			$data = mb_substr($data_, 0, $max_length_ - 3)."...";
		}
		else {
			$data = $data_;
		}

		return $data;
	}
}

