{extends file="main.tpl"}

{block name=seo}
	<title>お探しのページはみつかりません。(404 Not Found)</title>
{/block}

{block name=body}
	<div class="container not-found">
		<h2 class="font-color tecen"><span class="word-break">申し訳ありません。</span><span class="word-break">お探しのページは見つかりませんでした。</span><span class="word-break">(404 Not Found)</span></h2>
		<h3 class="tecen font-color">以下の原因が考えられます</h3>
		<ul class="tecen notfound-info">
			<li>1.URLの間違い</li>
			<li>2.占いランキングのデータが取得開始日以前<br>(総合運：{PREV_DATE|strtotime|date_format:"%Y/%m/%d"}以降、金運：{PREV_DATE_DTL_M|strtotime|date_format:"%Y/%m/%d"}以降、その他運勢は{PREV_DATE_DTL|strtotime|date_format:"%Y/%m/%d"}以降のデータをご確認いただけます。)</li>
		</ul>
		<h4 class="tecen "><a href = "/"><span class="not-found-button">ページのトップへ戻る</span></a></h4>
		<h4 class="tecen other-menu">各運勢ページはこちらから</h4>
			<div class="in-site-links">
				<div class="col-sm-6">
					<a href="/">
						<div class="in-site-integrated clearfix">
							<img class="col-xs-3 in-site-img" src="/user/img_re/defo-banner-character.png" alt="総合運">
							<div class="col-xs-9 in-site-txtsps">
								<h4 class="in-site-title">★総合運占い</h4>
								<p>総合運の占いランキングです。星座の個別ページではグラフや順位別のサイトリストもご覧いただけます。</p>
							</div>
						</div>
					</a>
				</div>
				<div class="col-sm-6">
					<a href="/love/">
						<div class="in-site-love clearfix">
							<img class="col-xs-3 in-site-img" src="/user/img_re/love-banner-character.png" alt="恋愛運">
							<div class="col-xs-9 in-site-txtsps">
								<h4 class="in-site-title">★恋愛運占い</h4>
								<p>恋愛運の占いランキングです。星座の個別ページではグラフや結果別のサイトリストもご覧いただけます。</p>
							</div>
						</div>
					</a>
				</div>
				<div class="col-sm-6">
					<a href="/work/">
						<div class="in-site-work clearfix">
							<img class="col-xs-3 in-site-img" src="/user/img_re/work-banner-character.png" alt="仕事運">
							<div class="col-xs-9 in-site-txtsps">
								<h4 class="in-site-title">★仕事運占い</h4>
								<p>仕事運の占いランキングです。星座の個別ページではグラフや結果別のサイトリストもご覧いただけます。</p>
							</div>
						</div>
					</a>
				</div>
				<div class="col-sm-6">
					<a href="{if $data.data_type != ""}/{$data.data_type}{/if}/ranking-past/">
						<div class="in-site-past clearfix">
							<img class="col-xs-3 in-site-img" src="/user/img_re/year-banner-character.png" alt="年間・月間">
							<div class="col-xs-9 in-site-txtsps">
								<h4 class="in-site-title">★月間・年間<br>ランキング</h4>
								<p>月間・年間の総合運と恋愛運の占いランキングがご覧いただけます。</p>
							</div>
						</div>
					</a>
				</div>
			</div>
	</div>
{/block}

