// definition of bost::hash_value for more params

namespace boost
{
	inline size_t hash_value(const CString cstring)
	{
		return hash_value(string(cstring));
	}

	inline size_t hash_value(const Udm::Object obj)
	{
        return obj.uniqueId();
	}
}