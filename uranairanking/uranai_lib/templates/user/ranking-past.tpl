{extends file="main.tpl"}
{block name=seo}
<title>12星座占い ランキング | 星占い {$year_past}年{if $month_past && $month_past != 'total'}{$month_past}月{/if}の{if $data.data_type != ""}{$data.topic_name} {/if}月間・年間ランキング</title>
<meta name="keywords" content="{if $data.data_type == ""}総合運{else}{$data.topic_name}{/if},占い,ランキング,月間,年間">
<meta name="description" content="12星座占いの順位を年間と月間で集計してランキング形式で一覧にしました！その月、その年にもっともいい傾向にある星座をチェックしてみましょう！毎月1日更新！">

<!--OGP START-->
{include file="ogp.tpl" title="|月間・年間ランキング{if $data.topic_name}($data.topic_name}){/if}" 
		des="12星座占いの順位を年間と月間で集計してランキング形式で一覧にしました！その月、その年にもっともいい傾向にある星座をチェックしてみましょう！毎月1日更新！"}
<!--OGP END--> 

{/block}


{block name=body}
<!--dropdown-->
<div class="modal fade {$data.data_type}" id="past-modal" tabindex="-1">
	<div class="modal-dialog">
		<div class="modal-content clearfix">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal"><span>×</span></button>
				<h4 class="modal-title tecen">年{if $month_past != 'total'}月{/if}を選択してください</h4>
			</div>
			<div class="modal-body clearfix">
				<div class="year">
				<ul class="list-unstyled" id="Accordion">
				{foreach from=$yearnav key=k item=y name=yearlist}
				{if $k != "tab"}
					<li class="year-li panel">
						{if $month_past == 'total'}
							<a href="{sitelink mode={$y}}">
						{else}
							<a class="year_pd collapsed" data-toggle="collapse" data-parent="#Accordion" href="#sampleAccordionCollapse{$k}">
						{/if}
						{$k}年</a>
						<div id="sampleAccordionCollapse{$k}" class="panel-collapse collapse">
							{if $k != "tab"}
								<ul class="m-{$k} list-unstyled">
									{foreach from=$y key=mn item=link}
										{if $mn == 'total'}{continue}{/if}
										<li class="month-li"><a href="{sitelink mode={$link}}">{$mn}月</a></li>
									{/foreach}
								</ul>
							{/if}
						</div>
					</li>
				{/if}
				{/foreach}
				</ul>
				</div>
			</div>
		</div>
	</div>
</div>
<!--/dropdown-->

<!-- コンテイナー -->
<div class="container {$data.data_type}">
	<div class="title row text-center font-color">
		<h2>{if $data.data_type != ""}{$data.topic_name} {/if}年間・月間ランキング</h2>
		{include "mainline.parts.tpl"}
	</div>
	{$announcement.ev_banner}
	<!-- メイン -->
	<div class="col-md-9 col-sm-12 container-ranking-past spadding-top">
		<ul class="r-past-tabs text-center reset clearfix">
			<li class="col-xs-6{if $month_past == "total"} active{/if}">
				<a class="base-bg" id="yearly_tab" {if $month_past != "total"}href="{sitelink mode={$select.tab}}"{/if}>年間<br class="hidden-lg">ランキング</a>
			</li>
			<li class="col-xs-6{if $month_past != "total"} active{/if}">
				<a class="base-bg" id="monthly_tab" {if $month_past == "total"}href="{sitelink mode={$select.tab}}"{/if}>月間<br class="hidden-lg">ランキング</a>
			</li>
		</ul>

		<div class="ranking-wrapper clearfix base-bg">
			<div class="select-area dropdown clearfix text-center">
				
				<button class="btn btn-default" data-toggle="modal" data-target="#past-modal">
					別の{if $month_past == 'total'}年{else}月{/if}を見る
				</button>
				
			</div>
			<h2 class="text-center">{$year_past}年{if $month_past && $month_past != 'total'}{$month_past}月{/if}</h2>
			<div class = "tecen">
				<ul class="reset">
					<li class="outer-li btn-group">
						<span class="year_pd btn btn-default dropdown-toggle" data-toggle="dropdown">他の運勢を見る<span class="caret"></span></span>
							<ul class="months dropdown-menu">
					{foreach from=$select key=k item=y}
						{if $k != "tab"}
									<li><a href="{sitelink mode={$y}}">{$t_jp[{$k}]}</a></li>
						{/if}
					{/foreach}
							</ul>
					</li>
				</ul>
			</div>
			<!--ランキング-->
			{if $rankingP.0.score}
			{foreach name=rankingloop from=$rankingP item=item}
			<div class="{if $smarty.foreach.rankingloop.index <= 2}col-sm-12{elseif $smarty.foreach.rankingloop.index < 9}col-xs-6{else}col-xs-4{/if} col-lg-4 star-card text-center">
				<div class="star-card-rank">{$item.num}</div>
				<h3>{$item.name}</h3>
				<img loading="lazy" src="/user/img_re/{$design_name.ev_name}{$item.en_name}.png" alt="星座占いサイトの{$item.name}キャラクター">
			</div>
			{if $smarty.foreach.rankingloop.index == 2}
			<!--広告-->
			<hr class="ad_sep">
			<div class="col-lg-12 adg ad-rpast">
				<div class="ad-bg modal-ad">
					{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
					{insert ad_group id="6"}
					{else}
					{insert ad_group id="5"}
					{/if}
				</div>
			</div>
			<!--/広告-->
			{/if}
 			{/foreach}
 			{else}
 				<div class="tecen">データがありません。</div>
 			{/if}
			<!--/ランキング-->
		<hr class="ad_sep">
		</div>
			<div class="adg-past-bottom">
				<div class="ad-bg modal-ad">
					{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
					{insert ad_group id="6"}
					{else}
					{insert ad_group id="4"}
					{/if}
				</div>
			</div>
			{insert loginInfo}
	</div>
	<!-- /メイン -->
	<!-- サイド -->
	{include file='sidebar.tpl'}
	<!-- /サイド -->
</div>
<!-- /コンテイナー -->
{/block}
