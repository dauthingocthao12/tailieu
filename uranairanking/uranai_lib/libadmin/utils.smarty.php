<?php

/**
 * DBの日付から日本語日付変換へ
 * 使い方： {$date_from_db|japanesedate}
 *
 * @author Azet
 * @param string $date_ (例: 2017-03-13)
 * @return string (例: 2017年03月13日)
 */
function smarty_modifier_japanesedate($date_) {
	$date_parts = explode(' ', $date_);
	//print_r($date_parts);
	$day_parts = explode('-', $date_parts[0]);

	// default
	$newDate = $date_;

	if($day_parts) {
		$newDate = $day_parts[0]."年".$day_parts[1]."月".$day_parts[2]."日";
	}

	return $newDate;
}


function smarty_modifier_japaneseDateFull($date_) {
	return preg_replace("/(\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})/", "$1年$2月$3日 $4:$5:$6", $date_);
}


/**
 * DBの日付からSEO用の日付変換へ
 * 使い方： {$date_from_db|seodate}
 *
 * @author Azet
 * @param string $date_ (例: 2017-03-13)
 * @return string (例: 2017/03/13)
 */
function smarty_modifier_seodate($date_) {
	$date_parts = explode(' ', $date_);
	//print_r($date_parts);

	// default
	$newDate = $date_;

	if($date_parts[0]) {
		$newDate = str_replace('-', '/', $date_parts[0]);
	}

	return $newDate;
}


/**
 * DBの日付からリンク用の日付変換へ
 * 使い方： {$date_from_db|linkdate}
 *
 * @author Azet
 * @param string $date_ (例: 2017-03-13)
 * @return string (例: 20170313)
 */
function smarty_modifier_linkdate($date_) {
	$date_parts = explode(' ', $date_);
	//print_r($date_parts);

	// default
	$newDate = $date_;

	if($date_parts[0]) {
		$newDate = str_replace('-', '', $date_parts[0]);
	}

	return $newDate;
}


/**
 * 最新お知らせ表示
 *
 * @author Azet
 * @return string HTML
 */
function smarty_insert_lastNews() {
	$news = News::getLast();
	// output
	$html = "";

	if($news && count($news)>0) {
		$html = "<ul class='news-list'>";
		foreach($news as $n) {
			$date_parts = explode(' ', $n['news_release_date']);
			$date = smarty_modifier_japanesedate($n['news_release_date']);
			$link = smarty_function_sitelink(array('mode' => 'whatnew/'.str_replace('-', '', $date_parts[0])."/".$n['news_id']));
			$html .= "<li><div class='news-date'>{$date}</div><a href='$link'>{$n['news_title']}</a></li>";
		}
		$html .= "</ul>";
	}

	return $html;
}


/**
 * ログインボタン
 * @return string HTML
 */
function smarty_insert_loginbtn() {
	$link_form = smarty_function_sitelink(array('mode' => 'mypage'));
	$link_login = smarty_function_sitelink(array('mode' => 'account/login'));
	$menu = "";

	if($user = Account::userInfos()) {
		$exclamation = smarty_insert_userCommentWarning();

		if (strlen($user['handlename']) > 0) {
			$menu .= "<a href='" . $link_form . "' role='button' class='login-btn btn btn-danger text-center pull-right hidden-xs force-wrap'>{$exclamation}マイページ (".$user['handlename']."さん)</a>";
		} else {
			$menu .= "<a href='" . $link_form . "' role='button' class='login-btn btn btn-danger text-center pull-right hidden-xs force-wrap'>{$exclamation}マイページ</a>";
		}
	} else {
		$menu .= "<a href='" . $link_login . "' role='button' class='login-btn btn btn-danger text-center pull-right hidden-xs force-wrap'>ログイン (新規登録)</a>";
	}
	return $menu;
}


/**
 * ユーザのボタン（ログイン・マイページ）
 * スマホ対応
 * 
 * @return string HTML
 */
function smarty_insert_loginbtnSP() {
	$entry = "";

	if($user = Account::userInfos()) {		
		$exclamation = smarty_insert_userCommentWarning();

		$link = smarty_function_sitelink(array('mode' => 'mypage'));
		$entry = '<li class="text-center"><a href="'.$link.'"><i class="fa fa-user" aria-hidden="true"></i><span class="icon-title">'.$exclamation.'マイページ</span></a></li>';
	}
	else {
		$link = smarty_function_sitelink(array('mode' => 'account/login'));
		$entry = '<li class="text-center"><a href="'.$link.'"><i class="fa fa-sign-in" aria-hidden="true"></i><span class="icon-title">ログイン</span></a></li>';
	}

	return $entry;
}


/**
 * ユーザのコメントにエラーがあれば、（！）マークを出す
 * 
 * @return string HTML
 */
function smarty_insert_userCommentWarning() {
	$user = Account::userInfos();
	$exclamation = "";

	if(SiteComment::getRejectedUserComments($user['user_id'])) {
		$exclamation = '<i class="fa fa-info-circle fa-shadow"></i> ';
	}
	
	return $exclamation;
}


// ハンバーガーボタン内ログイン
// 未使用
function smarty_insert_loginli() {
	$link_form = smarty_function_sitelink(array('mode' => 'mypage'));
	$link_login = smarty_function_sitelink(array('mode' => 'account/' . 'login'));
	$menu = "";
	if($user = Account::userInfos()) {
		if (strlen($user['handlename']) > 0) {
			$menu .= "<li class='force-wrap'><a href='" . $link_form . "'>マイページ (".$user['handlename']."さん)</a></li>";
		} else {
			$menu .= "<li class='force-wrap'><a href='" . $link_form . "'>マイページ</a></li>";
		}
	} else {
		$menu .= "<li class='force-wrap'><a href='" . $link_login . "'>ログイン (新規登録)</a></li>";
	}
	return $menu;
}


//会員登録バナー出力
function smarty_insert_loginInfo() {
	$member_induction = "";
	$link = smarty_function_sitelink(array('mode' => 'account/' . 'intro'));
	if (!Account::userInfos()) {
		$member_induction .= '<div class="spadding tecen">';
		$member_induction .= '<a href="' . $link . '">';;
		$member_induction .= '<img class="banner hidden-xs hidden-sm width-max" src="/user/img_re/member-registration-new-pc.png" alt="無料会員登録はこちら">';
		$member_induction .= '<img class="visible-xs visible-sm width-max" src="/user/img_re/member-registration-new.png" alt="無料会員登録はこちら">';
		$member_induction .= "</a>";
		$member_induction .= "</div>";
	}
	return $member_induction;
}


/**
 * ３つのランキングのリンク作成
 * 例： {insert 3dates_links mode="rank" prev="2017-03-21" curr="2017-03-22" next="2017-03-23" start=7}
 *
 * @author Azet
 * @param array $params_
 * @return string HTML
 */
function smarty_insert_3dates_links($params_) {
	$links = "";

	if($params_['prev']) {
		$p = $params_;
		$p['d'] = $params_['prev'];
		$link = smarty_function_sitelink($p);

		$links .= "<div class='col-xs-4 font_bold'>";
		if($p['topic'] =="money" && $p['d'] < PREV_DATE_DTL_M){
			$links .= "";
		}elseif($p['topic'] !="" && $p['d'] < PREV_DATE_DTL){
			$links .= "";
		}elseif($p['d'] < PREV_DATE){
			$links .= "";
		}else{
			$links .= "<a href='$link' class='rank-day-link-previous'><i class=\"fa fa-chevron-circle-left fa-lg spadding\" aria-hidden=\"true\"></i>前日</a>";
		}
		$links .= "</div>";
	}

	if($params_['curr']) {
		$p = $params_;
		$p['d'] = $params_['curr'];
		$link = smarty_function_sitelink($p);

		$links .= "<div class='col-xs-4 font_bold'>";
		$links .= "<a href='$link' class='rank-day-link-current'>今日</a>";
		$links .= "</div>";
	}

	if($params_['next']) {
		$p = $params_;
		$p['d'] = $params_['next'];
		$link = smarty_function_sitelink($p);

		$links .= "<div class='col-xs-4 font_bold'>";
		$links .= "<a href='$link' class='rank-day-link-next'>翌日<i class=\"fa fa-arrow-circle-right fa-lg spadding\" aria-hidden=\"true\"></i></a>";
		$links .= "</div>";
	}

	return $links;
}


function smarty_insert_previous_month_links($params_) {
	$links = "";
	//print_r($params_);
	if($params_['m']) {
		$p = $params_;
		$p['d'] = $params_['m'];
		if(empty($params_['star'])) {
			unset($p['star']);
		}
		if(empty($params_['topic'])) {
			unset($p['topic']);
		}
		//print_r($p);
		$link = smarty_function_sitelink($p);
		//print_r($p);print_r("ss".$link);

		$links .= "<a class='btn btn-default btn-xs cal_btn' href='$link'>&lt;前月</a>";
	}

	return $links;
}


function smarty_insert_next_month_links($params_) {
	$links = "";

	if($params_['m']) {
		$p = $params_;
		$p['d'] = $params_['m'];
		if(empty($params_['star'])) {
			unset($p['star']);
		}
		if(empty($params_['topic'])) {
			unset($p['topic']);
		}
		$link = smarty_function_sitelink($p);

		$links .= "<a class='btn btn-default btn-xs cal_btn' href='$link'>次月></a>";
	}

	return $links;
}


/**
 * サイト評価数字を★で表示 
 * 使い方： {insert siteEvaluationStars evaluation=4.21}
 *
 * @param array $params_
 * @return string HTML
 */
function smarty_insert_siteEvaluationStars($params_) {
	$evaluation = $params_['evaluation'];
	// print $evaluation."<br>";
	
	$min = floor($params_['evaluation']);
	$mid = $min+"0.5";
	$max = ceil($params_['evaluation']);
	// print $min."<".$mid."<".$max."<br>";

	$stars = "";

	for($i=1; $i<=5; ++$i) {
		if($i<=$min) {
			$stars .= '<i class="fa fa-star"></i>';
		}		
		elseif($i<=$max && $evaluation>=$mid) {
			$stars .= '<i class="fa fa-star-half-o"></i>';
		}
		else {
			$stars .= '<i class="fa fa-star-o"></i>';
		}
	}

	return $stars;
}


/**
 * コメントのステータス状態表示
 * 
 * @param string $status_ (published | refused | pending | hidden)
 * @return string HTML
 */
function smarty_modifier_commentStatusJapanese($status_) {
    $status_jp = array(
        'pending'   => '審査中',
        'published' => '公開中',
        'rejected'   => '無効',
        'hidden'    => '未公開'
    );
    
    return $status_jp[$status_];
}

/**
 * サイトデータから、urlのリンクを判断するテンプレート機能
 *
 * @author Azet
 * @param array $data_ (site_link_decide() 機能を参考)
 * @return string
 */
function smarty_modifier_siteLinkDecide($data_) {
	return site_link_decide($data_);
}


/**
 * サイトのコメントで、★のグラフを作る
 * 例： {insert siteStarDetails site_id=XXX}
 *
 * @author Azet
 * @param array $params_ [comments]
 * @return string HTML
 */
function smarty_insert_siteStarDetails($params_) {
	$graph = "";

	// stars counter
	$stars = array(
		1 => 0,
		2 => 0,
		3 => 0,
		4 => 0,
		5 => 0
	);
	$stars_sum = 0;

	$comments = array();
	if(isset($params_['site_id'])) {
		// サイトID
		$comments = SiteComment::getSiteEvaluationData($params_['site_id']);
	}
	elseif(isset($params_['comments'])) {
		// 情報をもらった
		$comments = $params_['comments'];
	}

	if(count($comments)>0) {
		// count per star
		$stars = array_reduce($comments, function($acc, $elem) {
			// evaluation
			$evaluation = $elem['evaluation'];
			$acc[$evaluation] = $acc[$evaluation] + 1;
			return $acc;
		}, $stars);
		// total of stars
		$stars_sum = array_sum($stars);

		$graph .= "<div class='site-ranking-container'>";
		for($i=5; $i>=1; --$i) {
			$star_data = $stars[$i];
			$percent = 100 * $star_data / $stars_sum;
			$percent_display = ceil($percent);
			$graph .= "
			<div class='progress-line'>
				<div class='progress-head'>$i ★</div>
				<div class='progress'>
					<div class='progress-bar' style='width:$percent%;'></div>
				</div>
				<div class='progress-tail text-right'>$star_data</div>
				<div class='progress-tail text-left'>($percent_display%)</div>
			</div>";
		}
		$graph .= "</div>";
	}
	else {
		$graph .= '<div class="alert alert-info">データがありません。</div>';
	}

	return $graph;
}


/**
 * サイトのコメントで、★のグラフを作る
 *
 * @author Azet
 * @param array $params_ [comments]
 * @return string HTML
 */
function smarty_insert_siteStarAverage($params_) {
	$html = "";

	// counter
	$rank_count = 0;
	$rank_sum   = 0;

	$comments = array();
	if(isset($params_['site_id'])) {
		// サイトID
		$comments = SiteComment::getSiteEvaluationData($params_['site_id']);
	}
	elseif(isset($params_['comments'])) {
		// 情報をもらった
		$comments = $params_['comments'];
	}

	if(count($comments)>0) {
		// count per star
		foreach($comments as $comm) {
			$rank_count += 1;
			$rank_sum += $comm['evaluation'];
		};
		$average = $rank_sum / $rank_count;

		// html generate
		$html .= '総合評価('. number_format($average, 1) .' / '. $rank_count .'件)';

		$average_stars = floor($average);
		$stars = smarty_insert_siteEvaluationStars(array('evaluation' => $average));
		
		$html .= "<div class='star'>$stars</div>";
	}
	else {
		$html .= '総合評価';
		$html .= '<div class="alert alert-info">データがありません。</div>';
	}

	return $html;
}

function smarty_modifier_findUserCommentPage($comment_) {
	return SiteComment::findUserCommentPage($comment_);
}


/**
 * 使い方： {insert userAvatar avatar="comment-icon-leo.png"}
 * 
 * @param  array $params_ ['avatar' => XXX]
 * @return string html img tag
 */
function smarty_insert_userAvatar($params_) {
	// default
	$avatar = "comment-icon-noimg.png";

	// user selection
	if(isset($params_['avatar']) && $params_['avatar']) {
		$avatar = $params_['avatar'];
	}

	// img tag
	$html = "<img src='/user/img_re/$avatar' alt='' class='img-circle desc-character' />";

	// output
	return $html;
}