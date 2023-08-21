{extends file="main.tpl"}
{block name=body}

<div class="text-right">
	{makelink mode="news" action="input" value="新規登録" class="btn btn-primary"}
</div>
<div class="clear">&nbsp;</div>

<table id="log-table" class="table table-striped table-condensed">
	<thead>
		<tr>
			<th>表示～</th>
			<th>公開日</th>
			<th>タイトル</th>
			<th class="text-center">表示</th>
			<th class="text-center">&nbsp;</th>
		</tr>
	</thead>
	<tbody>
		{foreach $data as $n}
		<tr {if $n.not_visible_yet}class="warning"{/if}>
			<td>{$n.promote_from_date|japanesedate}</td>
			<td>{$n.news_release_date|japanesedate}</td>
			<td>{$n.news_title}</td>
			<td class="text-center">
				{if $n.news_is_show=="1"}<i class="fa fa-eye"></i>{else}<i class="fa fa-eye-slash"></i>{/if}
			</td>
			<td class="text-right">
				{makelink mode="news" action="delete" id="{$n.news_id}" value="&#xf1f8; 削除" class="btn-fa btn btn-xs btn-danger"}
				{makelink mode="news" action="input" id="{$n.news_id}" value="&#xf040; 編集" class="btn-fa btn btn-xs btn-primary"}
				<a href="{sitelink mode="whatnew/{$n.news_release_date|linkdate}/{$n.news_id}"}" target="_blank" class="btn btn-xs btn-success">確認 <i class="fa fa-external-link"></i></a>
			</td>
		</tr>
		{/foreach}
	</tbody>
</table>
{/block}
