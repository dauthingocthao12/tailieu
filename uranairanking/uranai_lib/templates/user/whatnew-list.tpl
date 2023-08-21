{extends file="main.tpl"}
{block name=seo}
<title>新着情報 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,新着情報">
<meta name="description" content="新着情報。サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。">
<!--OGP START-->
{include file="ogp.tpl" title="|新着情報" des="新着情報。サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。"}
<!--OGP END-->
{/block}

{block name=body}

<div class="container whatnew">
	<div class="title row text-center">
		<h2 class="font-color">新着情報</h2>
		{include "mainline.parts.tpl"}
	</div>
	<div class="col-md-9">
			<hr class="ad_sep">
				{* 2023/05/19 お詫び発生につきコメントアウト *}
				{* <div class="ad-bg adg-news modal-ad">
					{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
					{insert ad_group id="6"}
					{else}
					{insert ad_group id="5"}
					{/if}
				</div> *}
				{* コメントアウトここまで *}
		<div class="col-xs-12 base-bg contents-space">

			<ul class="news-list">
				<!-- 内容 -->
				{foreach $news_list as $n}
				<li>
					<div class="news-date">{$n.url_date|japanesedate}</div>
					<a href="{sitelink mode="whatnew/{$n.url_date|linkdate}/{$n.news_id}"}">{$n.news_title}</a>
				</li>
				{/foreach}
			</ul>
			
			<!-- ページング -->
			<nav class="tecen">
				<ul class="pagination">
					<li>
					{if {$whatnew_page} != 1}
						<a href="{sitelink mode="whatnew/page{$whatnew_page-1}"}" aria-label="前のページへ">
							<span aria-hidden="true">«</span>
						</a>
					{/if}
					</li>
					{section name=page start=0 loop=$news_page_count} 
					<li class="{if $whatnew_page==$smarty.section.page.rownum}active{/if}">
						<a href="{sitelink mode="whatnew/page{$smarty.section.page.rownum}"}">{$smarty.section.page.rownum}</a>
					</li>
					{/section}
					<li>
					{if $whatnew_page < $news_page_count}
						<a href="{sitelink mode="whatnew/page{$whatnew_page+1}"}" aria-label="次のページへ">
							<span aria-hidden="true">»</span>
						</a>
					{/if}
					</li>
				</ul>
			</nav>
		</div>


		<hr class="ad_sep">
		{* 2023/05/19 お詫び発生につきコメントアウト *}
		{* <div class="ad-bg adg-news modal-ad">
			{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
			{insert ad_group id="6"}
			{else}
			{insert ad_group id="4"}
			{/if}
		</div> *}
		{* コメントアウトここまで *}
	</div>
	{include file='sidebar.tpl'}
</div>
{/block}
