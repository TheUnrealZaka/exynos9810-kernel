

#ifndef __INTEL_SVM_H__
#define __INTEL_SVM_H__

struct device;

struct svm_dev_ops {
	void (*fault_cb)(struct device *dev, int pasid, u64 address,
			 u32 private, int rwxp, int response);
};


#define SVM_REQ_READ	(1<<3)
#define SVM_REQ_WRITE	(1<<2)
#define SVM_REQ_EXEC	(1<<1)
#define SVM_REQ_PRIV	(1<<0)



#define SVM_FLAG_PRIVATE_PASID		(1<<0)


#define SVM_FLAG_SUPERVISOR_MODE	(1<<1)

#ifdef CONFIG_INTEL_IOMMU_SVM


extern int intel_svm_bind_mm(struct device *dev, int *pasid, int flags,
			     struct svm_dev_ops *ops);


extern int intel_svm_unbind_mm(struct device *dev, int pasid);

#else 

static inline int intel_svm_bind_mm(struct device *dev, int *pasid,
				    int flags, struct svm_dev_ops *ops)
{
	return -ENOSYS;
}

static inline int intel_svm_unbind_mm(struct device *dev, int pasid)
{
	BUG();
}
#endif 

#define intel_svm_available(dev) (!intel_svm_bind_mm((dev), NULL, 0, NULL))

#endif 
