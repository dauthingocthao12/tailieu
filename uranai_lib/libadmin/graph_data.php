<?php
// okabe 2016/08/23

function make_graph_data() {
	$today = date("Y-m-d");	//開始日付
	$modev = 0;	//取得モード 0:所定のファイル出力, 1:配列データで返す

	list($result, $dt) = make_graph1($today, $modev);	//集計実行モード
	//$result = false; $dt = array();	//何もしないモード

	return array($result, $dt);
}


//過去１週間分の星座別の順位変動
// 引数
//    $today: 開始日付
//    $modev: 取得モード 0:所定のファイル出力, 1:配列データで返す
// 出力
//   $modev=0 の場合
//     出力ファイル
//       tmpg/a-星座番号.txt
//     出力データ
//       last:2016-08-23			...指定されたデータ群の最新日付
//       update:2016-08-23 14:51:29	...最新日付データの最終更新時刻
//       count:8						...データ件数
//       data:6,11,4,4,8,12,9,6		...左から、古い日付順に、一番右が指定された最新日付
//   $modev=1 の場合
//     出力データ ※内容は上記それぞれの : の右部分と同じ
//       $data3[星座番号]['last']
//       $data3[星座番号]['update']
//       $data3[星座番号]['count']
//       $data3[星座番号]['data']
//
function make_graph1($today, $modev) {
	global $conn;

	$data = array();
	$data2 = array();
	$data3 = array();

	$count_data = 8;

	$limit_day = strftime('%Y-%m-%d', strtotime('-'.$count_data.' day', strtotime($today)));

	$sql = "SELECT * 
		FROM `score_daily` 
		WHERE `is_delete`=0
		AND `day`<='".$today."' 
		AND `day`>='".$limit_day."';";
	$rs = $conn->query($sql);
	if($rs) {
		while($row = $rs->fetch_assoc()) {
			$data[] = $row;
		}
	} else {
		return array(1, null);
	}

	//データを集計しやすく変換
	foreach($data as $ky => $dt) {
		$data2[$dt['day']][$dt['star']]['rank'] = $dt['rank'];
		if (strlen($dt['date_update']) > 0) {
			$data2[$dt['day']][$dt['star']]['update'] = $dt['date_update'];
		} else {
			$data2[$dt['day']][$dt['star']]['update'] = $dt['date_create'];
		}
	}


	if ($modev == 1) {	//配列で結果を返す
		for($i=1; $i<13; $i++) {	//星座コードのループ
			//グラフA用（過去１週間の順位変遷、本日含む８日間）
			for($j=0; $j<$count_data; $j++) {	//１週間のループ
				$hizuke = strftime('%Y-%m-%d', strtotime('-'.$j.' day', strtotime($today)));
				if ($j == 0) {
					$hz = $data2[$hizuke][$i]['update'];
					$data3[$i]['last'] = $hizuke;
					$data3[$i]['update'] = $hz;
					$data3[$i]['count'] = $count_data;
					$data3[$i]['data'] = "";
				}
				if ($j == 0) {
					$data3[$i]['data'] = $data2[$hizuke][$i]['rank'];
				} else {
					$data3[$i]['data'] = $data2[$hizuke][$i]['rank'].", ".$data3[$i]['data'];
				}
			}
		}
		return array(false, $data3);
	}
	else {	//ファイルを介して返す
		for($i=1; $i<13; $i++) {	//星座コードのループ
			//グラフ A（過去１週間の順位変遷、本日含む８日間）
			$nm = "a-".$i;
			$output_str = "";
			$output_str2 = "";
			for($j=0; $j<$count_data; $j++) {	//１週間のループ
				$hizuke = strftime('%Y-%m-%d', strtotime('-'.$j.' day', strtotime($today)));
				if ($j == 0) {
					$hz = $data2[$hizuke][$i]['update'];
					$output_str .= "last:".$hizuke."\n";
					$output_str .= "update:".$hz."\n";
					$output_str .= "count:".$count_data."\n";
					$output_str .= "data:";
				}
				if ($j == 0) {
					$output_str2 = $data2[$hizuke][$i]['rank'];
				} else {
					$output_str2 = $data2[$hizuke][$i]['rank'].",".$output_str2;
				}
			}
			$output_str .= $output_str2."\n";
			$output_str2 = "";
			//$output_str = "*\n";
			//$output_str .= print_r($data2, TRUE );
			//作成したデータ記述ファイルをテンポラリディレクトリに格納する bat/tmpg/下
			$file_path = GRAPH_TMP_FOLDER.$nm.".txt";
			file_put_contents($file_path, $output_str, LOCK_EX);
			chmod($file_path, 0666);
		}
	}

	return array(false, null);
}



//詳細ページにて、サービスグラフ部分の html を返す
function make_graph1_html($dt, $star) {
	global $name;
	$graph_data = "";
	$count_data = 8;

	$x = $dt[$star]['data'];
	$upd = $dt[$star]['update'];
	$upd_ary1 = explode(" ", $upd);
	if (count($upd_ary1) > 1) {
		$upd_ary2 = explode("-", $upd_ary1[0]);
		$upd_ary3 = explode(":", $upd_ary1[1]);

		if (count($upd_ary2) > 2 && count($upd_ary3) > 2) {
			if ($upd_ary3[0] > 12) {
				$jikoku = 12;
			} else {
				$jikoku = $upd_ary3[0];
			}
			$upd_str = "(".intval($upd_ary2[1])."月".intval($upd_ary2[2])."日 ".intval($jikoku)."時現在)";

			$st = $dt[$star]['last'];
			$labelx = "";
			for($j=0; $j<$count_data; $j++) {	//１週間のループ
				$hizukem = intval(strftime('%m', strtotime('-'.$j.' day', strtotime($st))));
				$hizuked = intval(strftime('%d', strtotime('-'.$j.' day', strtotime($st))));
				if ($j == 0) {
					$labelx = "'".$hizukem."/".$hizuked."'";
				} else {
					$labelx = "'".$hizukem."/".$hizuked."', ".$labelx;
				}
			}
			$datax = $x;

$graph_data = <<<EOM
<script src="/user/js/vendor/chartist.min.js"></script>
<link href="/user/css/vendor/chartist.min.css" rel="stylesheet" type="text/css" />
<!--[if IE 9]>
<script src="/user/js/vendor/matchMedia.js"></script>
<![endif]-->
<script type="text/javascript">
<!--
	 function init() {
		 var options = {
			 fullWidth: true,
				 chartPadding: {
				    right: 40
				 },
				 lineSmooth: Chartist.Interpolation.none({}),
			 axisY: {
				 labelInterpolationFnc: function(value) {
					 return -value;
				 },
				 labelOffset: {
					x: 0,
					y: 4
				 },
				 type: Chartist.FixedScaleAxis,
				 low: -12,
				 high: -1,
				 fullWidth: true,
				 ticks: [-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12],
				 onlyInteger: true
			 },
		 };
		 new Chartist.Line('#detail-graph-1', {
			 labels: [{$labelx}],
			 series: [[{$datax}]]
		 }
		 , options
		 ).on('data', function(context) {
			context.data.series = context.data.series.map(function(series) {
				return series.map(function(value) {
					return -value;
				});
			});
		}).on('draw', function(data) {
			// custom shape >>>
			// source for shape: http://www.smiffysplace.com/stars.html
			if(data.type === 'point') {
				// we are creating a new path svg element that draws a triangle around the point coordinates
				var triangle = new Chartist.Svg('path', {
					d: ["M",
						data.x,
						data.y + 5,
						"L",
						data.x + 5.878,
						data.y + 8.090,
						"L",
						data.x + 4.755,
						data.y + 1.545,
						"L",
						data.x + 9.511,
						data.y + -3.090,
						"L",
						data.x + 2.939,
						data.y + -4.045,
						"L",
						data.x + 0.000,
						data.y + -10.000,
						"L",
						data.x + -2.939,
						data.y + -4.045,
						"L",
						data.x + -9.511,
						data.y + -3.090,
						"L",
						data.x + -4.755,
						data.y + 1.545,
						"L",
						data.x + -5.878,
						data.y + 8.090,
						"L",
						data.x,
						data.y + 5,
						"Z"].join(' '),
					style: 'fill-opacity: 1'
				}, 'ct-area');

				// with data.element we get the chartist svg wrapper and we can replace the original point drawn by chartist with our newly created triangle
				data.element.replace(triangle);
			}
			// <<<
		});
	 };
	var sample = setTimeout( function(){ init(); }, 200);
-->
</script>
	<div id="detail-graph-1" class="ct-chart ct-octave detail_graph_panel"></div>
	<div class="detail_default_graph_datetime">
		{$upd_str}<br/>
	</div>
	<div class="detail_default_graph_desc">
		<span style="font-size: 13px;">
		※"前日"以前を表示しても、グラフは本日までの１週間分の表示になります。
		</span>
	</div>



EOM;
/*
<div class="alert alert-success text-center detail_default_graph">
	<div class="detail_default_graph_title">
		ここ１週間の {$name["star".$star]} の順位変化です。
	</div>
	<div class="detail_default_graph_desc">
	今後、グラフ機能を含めて、会員様向けの機能を益々充実させていきます♪<br/>
	もちろん、会員登録されても、これまで同様にご利用は<span class="detail_default_graph_red">無料</span>です。<br/>
	</div>
	<div class="detail_default_graph_desc">
	この機会にぜひ！会員登録されてみてはいかがでしょうか。<br/>
	</div>
	<div class="detail_default_graph_desc">
	かんたんステップで会員登録 ⇒ <a href="/account/intro" class="btn btn-primary btn-wrap">新規登録のご案内へ</a>
	</div>
</div>
*/
		}
	}

	return $graph_data;
}



