<?php
//12星座有料アプリ用 ajax通信
error_reporting(E_ALL ^ E_NOTICE);
// マスター設定
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/config.php");
// 共有ライブラリ
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/common.php");
// ランキング用クラス
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/uranairanking.class.php");

$html = "";
$plugin_run = false;

//パラメータによるランキングオブジェクトを生成　順位別サイト一覧データを配列に取得
$ranking = new UranaiRankingEx($_GET['date'],$_GET['data_type']);
$allRanks = $ranking->getAppDetailsForStarRankAjax($_GET['star'],$_GET['data_type']);

$plugin_time = $ranking->checkEarliestPluginTime();
$is_log = $ranking->isTodaysLog( $_GET['data_type'] );
//最も早いプラグインの実行時間を過ぎていたら
if($plugin_time < date("H:i:s")){
	$plugin_run = true;//プラグイン実行済
}

// 最高順位は表示したくない時用
//$best_rank = getBestRankOfStar($allRanks); //順位一覧から最高順位を取得
//unset($allRanks[$best_rank]); //最高順位の配列は削除する

//辛口アドバイスを見る || 日付遷移
if($_GET['action'] == 'karakuchi' || $_GET['action'] == 'move-date'){


	//グラフはどの日付ページにいても、本日から一週間のデータしか表示しない！
	$html .= "<div class='graph-title'>12星座での順位は以下のグラフで確認</div>";	
	$graph = $ranking->outputGraph(date('Ymd'),$_GET['star'],$_GET['data_type']);
	$html .= "<div class='graph-wrapper'>";	
	$html .= $graph;
	$html .= "<div class='graph-layer'>タップすると<span class='word'>グラフが表示されます</span></div>";
	$html .= "</div>";

	//日付表示
	$html .= "<div class='section-title'>".date('Y年m月d日',strtotime($_GET['date']))."の順位一覧</div>";
	//注意書き
	$html .= "<p class='link-alert'>サイト名の先頭に「※」が付いているサイトは過去の情報が閲覧出来ないため、<span class='word'>最新の情報へリンクしています。</span></p>";
	$html .= "<p class='font-alert'>※占いサイトによってはページが文字化けしてしまう場合がございます。<br>お手数でございますが、そのような場合には別のブラウザでご覧ください。<br><a href='http://minto.tech/iphone-mojibake/' target='_blank'><span class='alert-url'>「iPhoneのSafariで文字化けしてしまう<span class='word'>原因と解決方法」</span></span></a>";

	//運勢によって表記を変更
	if($_GET['data_type'] == ""){
		$rank_format = "位";
	}else{
		$rank_format = "点";
	}

	//logレコードが1件以上
	if($is_log){
		//サイト一覧があれば
		if($allRanks){
			//順位一覧生成
			foreach($allRanks as $rank => $sites){
				$html .= "<div class='karakuchi-rank content-closed' data-date='".$_GET['date']."' data-rank='".$rank."'>";
				$html .= "<span class='rank-header'>".$rank.$rank_format."</span>";
				$html .= "</div>";	
			}
		//表示可能なサイトがないとき
		}else{
			$html .= "<div class='in-progress'>";
			$html .= "<p>現在閲覧可能なサイトがありません。</p>";
			$html .= "</div>";
		}
	//logレコードが0件
	}else{
		//プラグインが実行済み
		if($plugin_run){
			$html .= "<div class='in-progress'>";
			$html .= "<p>集計データがありません。</p>";
			$html .= "</div>";
		//プラグイン実行前
		}else{
			$html .= "<div class='in-progress'>";
			$html .= "<p>現在データを集計中です。申し訳ありませんが時間をおいてからアクセスしてみてください。</p>";
			$html .= "</div>";
		}
	}

	//日付遷移リンク生成
	$html .= "<div id='move-date'>";
	$html .= "<ul class='list-inline'>";
	if($_GET['date'] > PREV_DATE){
		$html .= "<li><a id='d_prev' class='day-btn' data-date='".previous_date($_GET['date'])."' data-topic='".$data_type."'>前日</a></li>";
	}
	if($_GET['date'] != date('Ymd')){
		$html .= "<li><a id='d_today' class='day-btn' data-date='".date('Ymd')."' data-topic='".$data_type."'}>今日</a></li>";
	}
	if(next_date($_GET['date'])){
		$html .= "<li><a id='d_next' class='day-btn' data-date='".next_date($_GET['date'])."' data-topic='".$data_type."'>翌日</a></li>";
	}
	$html .= "</ul>";
	$html .= "</div>";
}	

//順位別サイトを見る
if($_GET['action'] == 'karakuchi-rank') {
	//サイト一覧生成
	$html .= "<ul class='sites clearfix'>";
	foreach($allRanks[$_GET['rank']] as $site){
		$html .= "<li>";
		$html .= "<a class='to-site' href='".$site['site_url']."' target='_blank'>".$site['site']."</a>";
		$html .= "</li>";
	}
	$html .= "</ul>";
}
//レスポンス
echo $html;
