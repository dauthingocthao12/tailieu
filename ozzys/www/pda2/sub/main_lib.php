<?PHP
/*

	ozzys様
	PDA商品管理システム

	基本設定ファイル

*/

	//	言語設定
	mb_language("Japanese");
	mb_internal_encoding("EUC-JP");

	//	セッションスタート
	session_start();

	//	文字コード
	## define("DISPLAY_ENCODE","EUC-JP");			//	変換表示文字コード
	## define("DISPLAY_SET_ENCODE","EUC-JP");		//	表示文字コード
	## define("SOURCE_ENCODE","EUC-JP");			//	ソース文字コード
	## define("DB_ENCODE","EUC_JP");				//	DB文字コード
    define("DISPLAY_ENCODE","UTF-8");
	define("DISPLAY_SET_ENCODE","UTF-8");		
	define("SOURCE_ENCODE","UTF-8");		
	define("DB_ENCODE","UTF-8");
	define("ITEM_DIR","/pda2");					//	作品表示システム基本リンク


	//	システムファイル
	define("INDEX_FILE","index.php");			//	TOPページ表示ファイル
	define("SEARCH_FILE","search.php");			//	商品検索ページ表示ファイル
	define("CHECK_FILE","check.php");			//	確認商品ページ表示ファイル


	//	イメージフォルダー
	define("DIR_IMG_ITEM","/pic/");				//	アイテムイメージフォルダー


	//	テンプレート
	define("INCLUDE_DIR","./template/");						//	テンプレートファイルフォルダー
	define("TEMP_INDEX","index.htm");							//	TOPひな形ファイル
	define("TEMP_SEARCH","search.htm");							//	検索ひな形ファイル
	define("TEMP_SEARCH_LIST","search_list.htm");				//	検索ひな形商品リストファイル
	//	add ookawara 2009/09/07
	define("TEMP_SEARCH_LIST2_HEAD","search_list2_head.htm");	//	検索ひな形共通商品ヘッドファイル
	define("TEMP_SEARCH_LIST2","search_list2.htm");				//	検索ひな形共通商品リストファイル

	define("TEMP_CHECK","check.htm");							//	確認商品ひな形ファイル
	define("TEMP_CHECK_LIST","check_list.htm");					//	確認商品ひな形商品リストファイル
	define("TEMP_PAGE","page.htm");								//	ページひな形ファイル


	//	DB情報
	define("DBHOST","localhost");					//	データーベースホスト名
	define("DBUSER","ozzys00001");					//	ユーザー名
	define("DBNAME","ozzys00001");					//	データーベース名
	define("DBPASS","Uf5mHmhv");					//	データーベースパスワード

	$TH = "";										//	DBNAME_HEAD
	define("TB_CLASS",$TH."class");					//	商品部門情報
	define("TB_GOODS",$TH."goods2");				//	商品基本情報
	define("TB_LIST",$TH."list");					//	商品詳細情報
	define("TB_MAKER",$TH."maker");					//	メーカー情報
	define("TB_PDA_SEARCH",$TH."pda_search");		//	検索情報
	define("TB_PDA_CHECK",$TH."pda_check");			//	商品チェック情報


	//	共通functionファイル読込
	include_once(SET_DIR."base_lib.php");			//	共通（ページ、エラー、pre）
	include_once(SET_DIR."change_code_html.php");	//	表示設定
	include_once(SET_DIR."db.php");					//	DB接続
	include_once(SET_DIR."read_html.class.php");	//	ひな形読込変換

?>