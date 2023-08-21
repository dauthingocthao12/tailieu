{extends file="main.tpl"}
{block name=seo}
<title>サイトの使い方 12星座占いランキング</title>
<meta name="keywords" content="12星座占いランキング,占い,ランキング,サイトの使い方">
<meta name="description" content="12星座占いランキングのサイトの使い方です。">

<!--OGP START-->
{include file="ogp.tpl" title="|サイトの使い方" des="12星座占いランキングのサイトの使い方です。"}
<!--OGP END--> 
{/block}
{block name=body}
<div class="container howtouse">
		<div class="title row text-center">
			<h2 class="font-color">{$config.plateform}の使い方</h2>
			{include "mainline.parts.tpl"}
		</div>
	<div class="base-bg clearfix">
		<div id="usemenu" class="row">
			{foreach from=$button key=c item=t name=loop}
				<div class="col-md-4 col-sm-6 touch-clr">
					<div class="mokuzi-button mokuzi-color{$smarty.foreach.loop.iteration}">
						<a href="#{$bttn_link.$t}">
							<div class="button-cell">
							<img class="use-mokuzi-img" src="/user/img_re/{$c}.png" alt="{$t}用ミニキャラ画像">
							</div>
							<div class="button-cell">
							<h3>{$t}</h3>
							</div>
						</a>
					</div>
				</div>
			{/foreach}
			
		</div>
		<h4 class="use-title t-section1 clearfix clear" id="top">TOPページについて<img class="use-title-img clearfix" src="/user/img_re/gemini-mini.png" height="50" alt="TOPページについて用ミニキャラ画像"></h4>
		<div class="row-height">
			<div class="col-md-6 m-margin-h tecen">
			<img class="useimg" src="/user/img_re/top-use.png" alt="星座占いのTOPページ画面画像">
			</div>
			<div class="col-md-6 m-margin-h">
				<div class="sub_title section1">
					<span class="n-title">
					①ナビゲーションバー
					</span>
				</div>
				<div class="nomal_text">
					それぞれのボタンから、各ページに遷移できます。<br>
					
				</div>
				<div class="sub_title section1">
					<span class="n-title">
					②ランキング結果
					</span>
				</div>
				<div class="nomal_text">
					各星座ランキングの全体の順位が一目でわかります。<br>
					各星座をクリックすると、その星座の詳細ページに移動できます。
				</div>
				<div class="sub_title section1">
					<span class="n-title">
					③日付確認カレンダー
					</span>
				</div>
				<div class="nomal_text">
					日付をクリックすれば指定の日付に遷移できます。<br>
					過去のデータは、2016年の１月までさかのぼることができます。				
				</div>
			</div>
			<div class="col-xs-12"><div class="section1-title">ナビゲーションバー詳細説明</div></div>
			<div class="col-md-6 m-margin-h tecen hidden-xs">
			<img class="useimg" src="/user/img_re/top-use-nav-pc.png" alt="星座占いのナビゲーションバーの画像(PC)">
			</div>
			<div class="col-md-6 m-margin-h hidden-xs">
				<div class="sub_title section1">
					<span class="n-title">
					①ページ上部のナビゲーションバー(PC)
					</span>
				</div>
				<div class="nomal_text">
					<dl>
						<dt>TOPへ</dt>
						<dd>全体運のランキングページへ移動します。<dd>
						<dt>各星座へ</dt>
						<dd>各星座の詳細情報ページに移動します。<dd>
						<dt>月間・年間</dt>
						<dd>過去の月間・年間ランキングページに移動できます<dd>
						<dt>総合運/恋愛運</dt>
						<dd>総合運・恋愛運の切り替えができます。<dd>
					</dl>
				</div>
				
				<div class="backbutton">
				<a href ="#usemenu">目次に戻る</a>
				</div>
			</div>
			<div class="col-md-6 m-margin-h tecen hidden-sm hidden-md hidden-lg">
			<img class="useimg" src="/user/img_re/top-use-nav-sp.png" alt="星座占いのナビゲーションバーの画像(スマホ)">
			</div>
			<div class="col-md-6 m-margin-h hidden-sm hidden-md hidden-lg">
				<div class="sub_title section1">
					<span class="n-title">
					①ページ上部のナビゲーションバー(タブレット・スマートフォン)
					</span>
				</div>
				<div class="nomal_text">
					<dl>
						<dt>HOME</dt>
						<dd>全体運のランキングページへ移動します。<dd>
						<dt>各星座名</dt>
						<dd>各星座の詳細情報ページに移動します。<dd>
						<dt>月間・年間ランキング</dt>
						<dd>過去の月間・年間ランキングページに移動できます<dd>
						<dt>ログイン(新規登録)</dt>
						<dd>ログイン・会員登録画面へ移動します。<dd>
					</dl>
				</div>
				<div class="sub_title section1">
					<span class="n-title">
					②ページ下部のナビゲーションバー(タブレット・スマートフォン)
					</span>
				</div>
				<div class="nomal_text">
					<dl>
						<dt>ランキング</dt>
						<dd>ランキング一覧ページに戻ります<dd>
						<dt>総合運/恋愛運</dt>
						<dd>総合運・恋愛運の切り替えができます。<dd>
						<dt>月間・年間</dt>
						<dd>過去の月間・年間ランキングページに移動できます<dd>
						<dt>ログイン(新規登録)</dt>
						<dd>ログイン・会員登録画面へ移動します。<dd>
					</dl>
				</div>

				
				<div class="backbutton">
				<a href ="#usemenu">目次に戻る</a>
				</div>
			</div>


		</div>
		
		<h4 class="use-title t-section2 clearfix clear" id="detail">星座詳細ページについて<img class="use-title-img clearfix" src="/user/img_re/capricorn-mini.png" height="50" alt="各星座詳細ページについて用ミニキャラ画像"></h4>
		<div class="row-height">
			<div class="col-md-6 m-margin-h tecen">
			<img class="useimg" src="/user/img_re/detail-use.png" alt="星座占いの各星座詳細ページ画面画像">
			</div>
			<div class="col-md-6 m-margin-h">
				<div class="sub_title section2">
					<span class="n-title">
					①星座情報
					</span>
				</div>
				<div class="nomal_text">
					星座の順位と、星座ごとの基本情報です。
				</div>
				<div class="sub_title section2">
					<span class="n-title">
					②順位変動グラフ
					</span>
				</div>
				<div class="nomal_text">
					１週間の運勢の変化をグラフとして見ることができます。
				</div>
				<div class="backbutton">
				<a href ="#usemenu">目次に戻る</a>
				</div>
			</div>
		</div>
		
		<h4 class="use-title t-section3 clearfix clear" id="year">年間・月間ページについて<img class="use-title-img clearfix" src="/user/img_re/cancer-mini.png" height="50" alt="年間・月間ページについて用ミニキャラ画像"></h4>
		<div class="row-height">
			<div class="col-md-6 m-margin-h tecen">
			<img class="useimg" src="/user/img_re/year-use.png" alt="星座占いの年間・月間ページ画面画像">
			</div>
			<div class="col-md-6 m-margin-h">
				<div class="sub_title section3">
					<span class="n-title">
					①年間・月間切り替えタブ
					</span>
				</div>
				<div class="nomal_text">
					年間単位の集計結果か、月間単位の集計結果かを選択できます。
				</div>
				<div class="sub_title section3">
					<span class="n-title">
					②年・月の選択
					</span>
				</div>
				<div class="nomal_text">
					・年間ページ<br>
					確認したい年度を選択してください<br>
					・月間ページ<br>
					確認したい年度と月を選択してください
				</div>
				<div class="backbutton">
				<a href ="#usemenu">目次に戻る</a>
				</div>
			</div>
		</div>
		
		<h4 class="use-title t-section4 clearfix clear" id="comment">コメント機能について<img class="use-title-img clearfix" src="/user/img_re/sagittarius-mini.png" height="50" alt="SNSについて用ミニキャラ画像"></h4>
		<div class="row-height">
			<div class="col-md-6 m-margin-h">
			<img class="useimg" src="/user/img_re/howtouse-c-1.jpg" alt="コメント機能投稿フォーム">
			<img class="useimg" src="/user/img_re/howtouse-c-2.jpg" alt="コメントイメージ">
			</div>
			<div class="col-md-6 m-margin-h">
				<div class="sub_title section4">
					<span class="n-title">
					①サイトコメント機能
					</span>
				</div>
				<div class="nomal_text">
					占いサイトについてのコメントを投稿できます。※会員登録者のみ
				</div>
				
				
				<div class="sub_title section4">
					<span class="n-title">
					②みんなのコメントを見る
					</span>
				</div>
				<div class="nomal_text">
					みんなが書いたコメントを一覧で見ることができます。<br>
					<a href="{sitelink mode="howtouse/comment"}">コメント機能の詳細説明はこちら!</a>
				</div>
				<br>
				<div class="backbutton">
				<a href ="#usemenu">目次に戻る</a>
				</div>
			</div>
			
		</div>
		
		<h4 class="use-title t-section5 clearfix clear"  id="login">ログイン・会員登録について<img class="use-title-img clearfix" src="/user/img_re/aeris-mini.png" height="50" alt="ログイン・会員登録について用ミニキャラ画像"></h4>
		<div class="row-height">
			<div class="col-md-6 m-margin-h tecen">
			<img class="useimg" src="/user/img_re/login-use.png" alt="星座占いのログイン・会員登録画面画像">
			</div>
			<div class="col-md-6 m-margin-h">
				<div class="sub_title section5">
					<span class="n-title">
					①ログイン・新規会員登録ボタン
					</span>
				</div>
				<div class="nomal_text">
					ログインページに移動できます。新規会員の方もこちらです。
				</div>
				<div class="sub_title section5">
					<span class="n-title">
					②ログイン情報入力
					</span>
				</div>
				<div class="nomal_text">
					会員登録済の方は登録したメールアドレスとパスワードを入力し、ログインできます。
				</div>
				<div class="sub_title section5">
					<span class="n-title">
					③新規会員登録ボタン
					</span>
				</div>
				<div class="nomal_text">
					新規で会員登録されたい方は、こちらのボタンをクリックしてください。				
				</div>
				<div class="backbutton">
				<a href ="#usemenu">目次に戻る</a>
				</div>
			</div>
		</div>
		<h4 class="use-title t-section6 clearfix clear" id="other">その他<img class="use-title-img clearfix" src="/user/img_re/virgo-mini.png" alt="その他用ミニキャラ画像"></h4>
		<div class="row-height">
			<div class="col-md-6 m-margin-h tecen">
			<img class="useimg" src="/user/img_re/year-month.png" alt="星座占いの過去の年間・月間ランキングをチェックバナー画像">
			</div>
			<div class="col-md-6 m-margin-h">
				<div class="sub_title section6">
					<span class="n-title">
					①年間・月間ランキング遷移バー
					</span>
				</div>
				<div class="nomal_text">
					こちらのバナーからも過去の月間・年間ランキングページに移動できます
				</div>
				
				
				<div class="sub_title section6">
					<span class="n-title">
					②各種運勢の取得日について
					</span>
				</div>
				<div class="nomal_text">
					以下の日付のデータをご確認いただけます。
					<dl>
					  <dt>総合運</dt><dd>{PREV_DATE|strtotime|date_format:"%Y/%m/%d"}以降</dd>
					  <dt>恋愛運</dt><dd>{PREV_DATE_DTL|strtotime|date_format:"%Y/%m/%d"}以降</dd>
					  <dt>仕事運</dt><dd>{PREV_DATE_DTL|strtotime|date_format:"%Y/%m/%d"}以降</dd>
					  <dt>金運</dt><dd>{PREV_DATE_DTL_M|strtotime|date_format:"%Y/%m/%d"}以降</dd>
					</dl>
				</div>
				<div class="backbutton">
				<a href ="#usemenu">目次に戻る</a>
				</div>
			</div>
		</div><!--end row-height-->

		
	</div><!--end base-bg-->
</div>
{/block}
