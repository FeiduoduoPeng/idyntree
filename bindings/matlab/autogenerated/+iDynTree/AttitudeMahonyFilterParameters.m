classdef AttitudeMahonyFilterParameters < SwigRef
  methods
    function this = swig_this(self)
      this = iDynTreeMEX(3, self);
    end
    function varargout = time_step_in_seconds(self, varargin)
      narginchk(1, 2)
      if nargin==1
        nargoutchk(0, 1)
        varargout{1} = iDynTreeMEX(1687, self);
      else
        nargoutchk(0, 0)
        iDynTreeMEX(1688, self, varargin{1});
      end
    end
    function varargout = kp(self, varargin)
      narginchk(1, 2)
      if nargin==1
        nargoutchk(0, 1)
        varargout{1} = iDynTreeMEX(1689, self);
      else
        nargoutchk(0, 0)
        iDynTreeMEX(1690, self, varargin{1});
      end
    end
    function varargout = ki(self, varargin)
      narginchk(1, 2)
      if nargin==1
        nargoutchk(0, 1)
        varargout{1} = iDynTreeMEX(1691, self);
      else
        nargoutchk(0, 0)
        iDynTreeMEX(1692, self, varargin{1});
      end
    end
    function varargout = use_magnetometer_measurements(self, varargin)
      narginchk(1, 2)
      if nargin==1
        nargoutchk(0, 1)
        varargout{1} = iDynTreeMEX(1693, self);
      else
        nargoutchk(0, 0)
        iDynTreeMEX(1694, self, varargin{1});
      end
    end
    function varargout = confidence_magnetometer_measurements(self, varargin)
      narginchk(1, 2)
      if nargin==1
        nargoutchk(0, 1)
        varargout{1} = iDynTreeMEX(1695, self);
      else
        nargoutchk(0, 0)
        iDynTreeMEX(1696, self, varargin{1});
      end
    end
    function self = AttitudeMahonyFilterParameters(varargin)
      if nargin==1 && strcmp(class(varargin{1}),'SwigRef')
        if ~isnull(varargin{1})
          self.swigPtr = varargin{1}.swigPtr;
        end
      else
        tmp = iDynTreeMEX(1697, varargin{:});
        self.swigPtr = tmp.swigPtr;
        tmp.SwigClear();
      end
    end
    function delete(self)
      if self.swigPtr
        iDynTreeMEX(1698, self);
        self.SwigClear();
      end
    end
  end
  methods(Static)
  end
end