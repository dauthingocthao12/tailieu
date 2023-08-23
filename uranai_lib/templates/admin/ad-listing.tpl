{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
<script type="text/javascript">
function check(msg, emsg, href) {
	var res = confirm(msg);
	if( res == false ) {
		alert(emsg);
	}
	return res
}
</script>

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
	<div class="pull-left">
		<form action="" method="post">
			<input type="hidden" name="mode" value="ad">
			<input type="hidden" name="action" value="listing">
			<label class="radio-label">
				<input type="radio" name="filter" value="all" {if $data.filter=='all'} checked{/if}> 使用
			</label>
			<label class="radio-label bg-success">
				<input type="radio" name="filter" value="active" {if $data.filter=='active'} checked{/if}> 使用
			</label>
			<label class="radio-label bg-danger">
				<input type="radio" name="filter" value="off" {if $data.filter=='off'} checked{/if}> 非表示
			</label>
			<label class="radio-label bg-warning">
				<input type="radio" name="filter" value="over" {if $data.filter=='over'} checked{/if}> 使用終了
			</label>
			<input type="submit" class="btn btn-sm btn-primary" value="絞り込む">
		</form>
	</div>

	<div class="pull-right">
		{makelink mode="ad" action="input" value="新規登録" class="btn btn-primary"}
	</div>
</div>
<div class="clear">&nbsp;</div>

<table class="table table-striped">
	<thead>
		<tr>
			<th>ID</th>
			<th>広告名</th>
			<th class="nowrap text-center">クリック数<hr>表示数</th>
			<th class="text-center">開始日時<hr>終了日時</th>
			<th class="text-center">表示</th>
			<th width="1%" class="text-center">操作</th>
		</tr>
	</thead>
	{if $data.db}
	<tbody>
		{foreach $data.db as $datum}
		<tr class="{if $datum.is_over=='1'} warning{/if}">
			<td>{$datum.ad_id}</td>
			<td>{$datum.ad_name}</td>
			<td class="text-center">{$datum.count_click}<hr>{$datum.count_display}</td>
			<td class="nowrap text-center">{$datum.ad_date_begin}<hr>{$datum.ad_date_end}</td>
			<td class="text-center">
				{if $datum.ad_is_show}
					<span class="label label-success">[ON]</span>
				{else}
				<span class="label label-danger">[OFF]</span>
				{/if}
			</td>
			<td class="nowrap">
				{makelink mode="ad" action="delete" id="{$datum.ad_id}" value="削除" class="btn btn-danger"}
				{makelink mode="ad" action="input" id="{$datum.ad_id}" value="編集" class="btn btn-primary"}
				{makelink mode="ad" action="detail" id="{$datum.ad_id}" value="詳細" class="btn btn-success"}
			</td>
		</tr>
		{/foreach}
	</tbody>
	{/if}
</table>
{/block}
