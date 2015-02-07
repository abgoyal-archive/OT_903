

#ifndef _NET_CLS_CGROUP_H
#define _NET_CLS_CGROUP_H

#include <linux/cgroup.h>
#include <linux/hardirq.h>
#include <linux/rcupdate.h>

#ifdef CONFIG_CGROUPS
struct cgroup_cls_state
{
	struct cgroup_subsys_state css;
	u32 classid;
};

#ifdef CONFIG_NET_CLS_CGROUP
static inline u32 task_cls_classid(struct task_struct *p)
{
	if (in_interrupt())
		return 0;

	return container_of(task_subsys_state(p, net_cls_subsys_id),
			    struct cgroup_cls_state, css)->classid;
}
#else
extern int net_cls_subsys_id;

static inline u32 task_cls_classid(struct task_struct *p)
{
	int id;
	u32 classid = 0;

	if (in_interrupt())
		return 0;

	rcu_read_lock();
	id = rcu_dereference(net_cls_subsys_id);
	if (id >= 0)
		classid = container_of(task_subsys_state(p, id),
				       struct cgroup_cls_state, css)->classid;
	rcu_read_unlock();

	return classid;
}
#endif
#else
static inline u32 task_cls_classid(struct task_struct *p)
{
	return 0;
}
#endif
#endif  /* _NET_CLS_CGROUP_H */