{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}

<div>
	<div class="pull-left" style="margin-right:10px;">
		<form action="" method="post">
			<input type="submit" class="btn btn-sm btn-primary" value="広告一覧">
			<input type="hidden" name="mode" value="ad">
			<input type="hidden" name="action" value="listing">
		</form>
	</div>
	<div class="pull-left">
		<form action="" method="post">
			<input type="submit" class="btn btn-sm btn-primary" value="広告グループ一覧">
			<input type="hidden" name="mode" value="ad">
			<input type="hidden" name="action" value="group-listing">
		</form>
	</div>
</div>
<div class="clear">&nbsp;</div>

<div>
	<div class="pull-right">
		{makelink mode="ad" action="group-input" value="新規登録" class="btn btn-primary"}
	</div>
</div>
<div class="clear">&nbsp;</div>

<table class="table table-striped">
	<thead>
		<tr>
			<th>ID</th>
			<th>広告グループ名</th>
			<th width="1%" class="text-center">操作</th>
		</tr>
	</thead>
	{if $data.db}
	<tbody>
		{foreach $data.db as $data}
		<tr>
			<td>{$data.ad_group_id}</td>
			<td>{$data.ad_group_name}</td>
			<td class="nowrap">
				{makelink mode="ad" action="group-delete" id="{$data.ad_group_id}" value="削除" class="btn btn-danger"}
				{makelink mode="ad" action="group-input" id="{$data.ad_group_id}" value="編集" class="btn btn-primary"}
				{makelink mode="ad" action="group-detail" id="{$data.ad_group_id}" value="詳細" class="btn btn-success"}
			</td>
		</tr>
		{/foreach}
	</tbody>
	{/if}
</table>

<style>
	#ad_pos_table td,
	#ad_pos_table th{
		font-size:12px;
	}

	#ad_pos_table{
		border:1px solid #e0e0e0;
	}
</style>

{assign var="last_id" value=""}
<hr>
<p>使用箇所</p>
<table id="ad_pos_table" border="1">
<tr><th>広告グループID</th><th>ファイル</th><th>行</th></tr>
{foreach $ad_files as $group_id => $files}
	{foreach $files as $file_name => $lines}
		<tr {if $group_id != $last_id }style="border-top:1px solid #b1b1b1;"{/if}>
			<td>{$group_id}</td><td>{$file_name}</td><td>{implode(", ",$lines)}</td>
		</tr>
		{assign var="last_id" value="$group_id"}
	{/foreach}
{/foreach}
</table>

{/block}
