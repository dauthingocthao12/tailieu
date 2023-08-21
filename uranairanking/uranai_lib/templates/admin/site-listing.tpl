{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
{* TODO: LISTING *}
<script type="text/javascript">
function check(msg, emsg, href) {
	var res = confirm(msg);
	if( res == false ) {
		alert(emsg);
	}
	return res
}
</script>

<div class="text-right">
	{makelink mode="site" action="input" value="新規登録" class="btn btn-primary"}
</div>
<br>

{if $data.db}
<table class="table table-striped">
	<thead>
		<tr>
			<th class="nowrap">サイト ID</th>
			<th>登録サイト名 <span class="badge">親番号</span></th>
			<th class="text-center">取得時間</th>
			<th class="text-center">バッチ</th>
			<th class="text-center">トピックバッチ</th>
			<th class="text-center">バッチ起動</th>
			<th class="text-center">操作</th>
		</tr>
	</thead>
	<tbody>
		{foreach $data.db as $datum}
		<tr>
			<td>{$datum.site_id}</td>
			<td>{$datum.site_name}{if $datum.parent_id>0} <span class="badge">[{$datum.parent_id}] {$datum.parent_name}</span>{/if} </td>

			<td class="nowrap text-center">
				{$datum.site_get_time}
			</td>

			<td class="nowrap text-center">
			{if $datum.is_execute}
				<span class="label label-success">[ON]</span>
			{else}
				<span class="label label-danger">[OFF]</span>
			{/if}

			{* {if $datum.batch} *}
			{* 	{makelink mode="site" action="batch" id=$datum.site_id value="起動" class="btn btn-default"} *}
			{* {/if} *}
			<!--{if $datum.batch_test}
				{makelink mode="site" action="batch-test" id=$datum.site_id value="test起動" class="btn btn-default"}
			{/if}-->
			{* 危ないので無効化 *}
			{* {if $datum.batch_topic} *}
			{* 	{makelink mode="site" action="batch-topic" id=$datum.site_id value="個別起動" class="btn btn-default"} *}
			{* {/if} *}
			</td>

			<td class="nowrap text-center">
			{if $datum.site_topic}
				<span class="label label-success">[ON]</span>
			{else}
				<span class="label label-danger">[OFF]</span>
			{/if}
			</td>

			<td class="nowrap text-center">
				{if $datum.batch}
					{makelink mode="site" action="batch" id=$datum.site_id value="起動" class="btn btn-default"}
				{/if}
			</td>

			<td class="nowrap">
				{makelink mode="site" action="delete" id="{$datum.site_id}" value="削除" class="btn btn-danger"}
				{makelink mode="site" action="input" id="{$datum.site_id}" value="編集" class="btn btn-primary"}
				{makelink mode="site" action="detail" id="{$datum.site_id}" value="詳細" class="btn btn-success"}
			</td>
		</tr>
		{/foreach}
	</tbody>
</table>
{/if}
{/block}
