{extends file="main.tpl"}
{block name=seo}
<title>占いサイト一覧 12星座占いランキング</title>
<meta name="keywords" content="12星座占いランキング,占い,ランキング,サイト一覧">
<meta name="description" content="12星座占いランキングのサイト一覧です。">

<!--OGP START-->
{include file="ogp.tpl" title="|占いサイト一覧" des="12星座占いランキングのサイト一覧です。"}
<!--OGP END-->
{/block}

{block name=body}
<div class="site-list">
	<div class="title row tecen">
		<h2 class="font-color">占いサイト一覧</h2>
		{include "mainline.parts.tpl"}
	</div>
	<div class="col-xs-12 tecen base-bg contents-space">
		占いサイト運営者様<br />
		集計に加えるサイトは順次追加しております。<br />
		独自に占いランキングサイトを運営されているサイト管理者様で当ランキング{$config.plateform}にご興味をお持ちいただいた際には追加させて頂きますのでお気軽にご連絡下さい。<br />
		<a href="mailto:info@uranairanking.jp">info@uranairanking.jp</a><br />
		※ランキングサイトの構成によっては追加できない場合がございます。
	</div>


	<div class="col-xs-12 base-bg itiran-pg contents-space base-bg-margin">
		<div class="normal-frame">
			<h4 class="tecen">それぞれのサイトの感想を投稿しましょう！</h4>
			<a href="{sitelink mode="howtouse/comment"}"><p class="tecen">投稿方法はこちら</p></a>
		</div>

		<div id="site-lists" class="clearfix">
			<div class="col-sm-6 col-xs-12">
				<ul class="ranking-sites table">
					{foreach $sitelinks as $name => $link}
					<li class="table-row">
						<div class="table-cell">
							<a href="{sitelink mode="site-description/{$link.site_id}"}">{$name}</a>
						</div>
						<div class="site-rank-data table-cell">
						{if $sites_ranking[$link.site_id]}
							<a href="{sitelink mode="site-description/{$link.site_id}"}">
								<div class="visible-sm visible-md visible-lg">
									<!-- pc + -->
									{insert siteEvaluationStars evaluation=$sites_ranking[$link.site_id].evaluation_average}
									{$sites_ranking[$link.site_id].evaluation_average|number_format:1}
									<i class="fa fa-commenting-o fa-flip-horizontal"></i> {$sites_ranking[$link.site_id].comments_count}
								</div>
								<div class="visible-xs">
									<!-- phone -->
									<span class="fa-stack">
										<i class="fa fa-stack-2x fa-star"></i>
										<span class="fa-stack-1x evaluation-number">{$sites_ranking[$link.site_id].evaluation_average|number_format:1}</span>
									</span>
									<small>(<i class="fa fa-commenting-o fa-flip-horizontal"></i> {$sites_ranking[$link.site_id].comments_count})</small>
								</div>
							</a>
						{else}
							<a href="{sitelink mode="site-description/{$link.site_id}"}" class="site-description-link"><span class="hidden-xs">このサイトを</span>評価する</a>
						{/if}
						</div>
					</li>
					{if $link@iteration==$count}
				</ul>
			</div>
			<div class="col-sm-6 col-xs-12">
				<ul class="ranking-sites table">
					{/if}
					{/foreach}
				</ul>
				<div class="clear col-sm-12 text-right" id="site-sorting">※サイトは上からあいうえお順で表示しています※</div>
			</div>
		</div>

		<div class="text-center sougo-link-heading normal-frame">
			<h4>相互リンク集</h4>
			相互リンクして頂いているサイト様の紹介です。<br>
			相互リンクは随時募集中です！ <br>
			連絡 <a href="mailto:info@uranairanking.jp">info@uranairanking.jp</a> までお願い致します。
		</div>
		{if count($sougo_link_list) == 0}
			<p class="text-center">現在、相互リンクを随時募集中です！</p>
		{else}
			<div id="site-lists" class="clearfix">
				<div class="col-sm-6 col-xs-12">
					<ul class="ranking-sites table sougo-link">
						{foreach $sougo_link_list as $site}
						<li class="table-row">
							<div class="table-cell">
								<a href="{$site["their_link"]}" target="_blank">{$site["site_name"]}</a>
							</div>
						</li>
						{if $site@iteration== $sougo_list_count}
					</ul>
				</div>
				<div class="col-sm-6 col-xs-12">
					<ul class="ranking-sites table sougo-link">
						{/if} 
						{/foreach}
					</ul>
				</div>
			</div>
			<div class="clear col-sm-12 text-right">※サイトは上からあいうえお順で表示しています※</div>
		{/if}
	</div>

</div>


{/block}
